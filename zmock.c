/**
 * ZMock unit test mock framework.
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "zmock.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys/queue.h>

struct zmock_mock_object {
	enum zmock_type type;
	const char *name;
	int count;
	union {
		zmock_value val;
		zmock_func wrapper;
	};
	const char *file;
	int line;
	const char *func;
	TAILQ_ENTRY(zmock_mock_object) entry;
};

static TAILQ_HEAD(, zmock_mock_object) g_zmock_objects;

__attribute__((constructor)) static void zmock_objects_init(void)
{
        TAILQ_INIT(&g_zmock_objects);
}

void _zmock_will_return(const char *name, zmock_value val, int count,
			const char *file, int line, const char *func)
{
	struct zmock_mock_object *obj;

	obj = calloc(1, sizeof(*obj));
	assert(obj);
	obj->type = zmock_type_return;
	obj->name = name;
	obj->count = count;
	obj->val  = val;
	obj->file = file;
	obj->line = line;
	obj->func = func;

	TAILQ_INSERT_TAIL(&g_zmock_objects, obj, entry);
}

void _zmock_will_call(const char *name, zmock_func wrapper, int count,
		      const char *file, int line, const char *func)
{
	struct zmock_mock_object *obj;

	obj = calloc(1, sizeof(*obj));
	assert(obj);
	obj->type = zmock_type_call;
	obj->name = name;
	obj->count = count;
	obj->wrapper = wrapper;
	obj->file = file;
	obj->line = line;
	obj->func = func;

	TAILQ_INSERT_TAIL(&g_zmock_objects, obj, entry);
}

static struct zmock_mock_object *zmock_find_mock_object(const char * const name)
{
        struct zmock_mock_object *obj;

        TAILQ_FOREACH(obj, &g_zmock_objects, entry) {
                if (!strcmp(obj->name, name))
                	return obj;
        }
        return NULL;
}

static void zmock_remove_mock_object(struct zmock_mock_object *obj)
{
	TAILQ_REMOVE(&g_zmock_objects, obj, entry);
}

void _zmock_will_reset(const char *name)
{
        struct zmock_mock_object *obj = zmock_find_mock_object(name);

	if (obj) {
		zmock_remove_mock_object(obj);
		free(obj);
	}
}

enum zmock_type _zmock_mock_type(const char *name)
{
        struct zmock_mock_object *obj = zmock_find_mock_object(name);

	return obj != NULL ? obj->type : zmock_type_none;
}

zmock_value _zmock_mock_value(const char *name)
{
	zmock_value val;
        struct zmock_mock_object *obj = zmock_find_mock_object(name);

	assert(obj);
	assert(obj->type == zmock_type_return);
	assert(obj->count > 0);
	obj->count -= 1;
	val = obj->val;
	if (!obj->count) {
		zmock_remove_mock_object(obj);
		free(obj);
	}
	return val;
}

zmock_func _zmock_mock_func(const char *name)
{
	zmock_func wrapper;
        struct zmock_mock_object *obj = zmock_find_mock_object(name);

	assert(obj);
	assert(obj->type == zmock_type_call);
	assert(obj->count > 0);
	obj->count -= 1;
	wrapper = obj->wrapper;
	if (!obj->count) {
		zmock_remove_mock_object(obj);
		free(obj);
	}
	return wrapper;
}
