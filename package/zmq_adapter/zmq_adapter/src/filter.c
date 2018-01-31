/*
 * Copyright (C) 2016-2018 Swift Navigation Inc.
 * Contact: Swift Navigation <dev@swiftnav.com>
 *
 * This source is subject to the license found in the file 'LICENSE' which must
 * be be distributed together with this source. All other rights reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "filter.h"
#include "logging.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/queue.h>

#include <libpiksi/logging.h>

#include <libsbp/sbp.h>
#include <libsbp/settings.h>

typedef struct filter_interface_s {
  const char *name;
  filter_create_fn_t create;
  filter_destroy_fn_t destroy;
  filter_process_fn_t process;
  SLIST_ENTRY(filter_interface_s) next;
} filter_interface_t;

typedef SLIST_HEAD(filter_interface_head_s, filter_interface_s) filter_interface_head_t;

static filter_interface_head_t filter_interface_list = SLIST_HEAD_INITIALIZER(filter_interface_list);

typedef struct filter_s {
  void *state;
  filter_interface_t *interface;
  SLIST_ENTRY(filter_s) next;
} filter_t;

typedef SLIST_HEAD(filter_list_s, filter_s) filter_list_t;

static filter_interface_t * filter_interface_lookup(const char *name)
{
  filter_interface_t *interface;

  SLIST_FOREACH(interface, &filter_interface_list, next) {
    if (strcasecmp(name, interface->name) == 0) {
      return interface;
    }
  }

  return NULL;
}

int filter_interface_register(const char *name,
                              filter_create_fn_t create,
                              filter_destroy_fn_t destroy,
                              filter_process_fn_t process)
{
  filter_interface_t *interface = (filter_interface_t *)
                                      malloc(sizeof(*interface));

  if (interface == NULL) {
    zmq_adapter_log(LOG_ERR, "error allocating filter interface");
    return -1;
  }

  *interface = (filter_interface_t) {
    .name = strdup(name),
    .create = create,
    .destroy = destroy,
    .process = process,
    .next = { NULL }
  };

  if (interface->name == NULL) {
    zmq_adapter_log(LOG_ERR, "error allocating filter interface members");
    free(interface);
    interface = NULL;
    return -1;
  }

  SLIST_INSERT_HEAD(&filter_interface_list, interface, next);

  return 0;
}

int filter_interface_valid(const char *name)
{
  filter_interface_t *interface = filter_interface_lookup(name);
  if (interface == NULL) {
    return -1;
  }
  return 0;
}

static filter_t * filter_create_one(const char* name, const char* filename)
{
  /* Look up interface */
  filter_interface_t *interface = filter_interface_lookup(name);
  if (interface == NULL) {
    zmq_adapter_log(LOG_ERR, "unknown filter: %s", name);
    return NULL;
  }

  filter_t *filter = (filter_t *)malloc(sizeof(*filter));
  if (filter == NULL) {
    zmq_adapter_log(LOG_ERR, "error allocating filter");
    return NULL;
  }

  *filter = (filter_t) {
    .state = interface->create(filename),
    .interface = interface
  };

  if (filter->state == NULL) {
    zmq_adapter_log(LOG_ERR, "error creating filter");
    free(filter);
    filter = NULL;
    return NULL;
  }

  return filter;
}

filter_list_t * filter_create(filter_spec_t specs[], size_t spec_count)
{
  filter_t *filter = NULL;

  filter_list_t* list = malloc(sizeof(*list));
  SLIST_INIT(list);

  for (size_t x = 0; x < spec_count; x++) {

    filter = filter_create_one(specs[x].name, specs[x].filename);
    if (filter == NULL)
      goto filter_create_error;

    SLIST_INSERT_HEAD(list, filter, next);
  }

  return list;

filter_create_error:
  SLIST_FOREACH(filter, list, next) {
    free(filter);
  }

  free(list);

  return NULL;
}

void filter_destroy(filter_list_t **filter_list)
{
  assert(filter_list != NULL);

  if (*filter_list == NULL)
    return;

  while(!SLIST_EMPTY(*filter_list)) {
    filter_t* filter = SLIST_FIRST(*filter_list);
    SLIST_REMOVE_HEAD(*filter_list, next);
    filter->interface->destroy(&filter->state);
    free(filter);
  }

  free(*filter_list);
  *filter_list = NULL;
}

int filter_process(filter_list_t *filter_list, const uint8_t *msg, uint32_t msg_length)
{
  filter_t *filter = NULL;

  SLIST_FOREACH(filter, filter_list, next) {
    int reject = filter->interface->process(filter->state, msg, msg_length);
    if (reject != 0)
      return reject;
  }

  return 0;
}