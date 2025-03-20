#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stddef.h>

typedef void (*task_func)(void* arg);

typedef struct {
    task_func func;
    void* arg;
} task_t;

typedef struct thread_pool thread_pool_t;

// Create a thread pool with a given number of threads.
thread_pool_t* thread_pool_create(size_t num_threads);
// Enqueue a task to the thread pool.
void thread_pool_enqueue(thread_pool_t* pool, task_func func, void* arg);
// Destroy the thread pool and free resources.
void thread_pool_destroy(thread_pool_t* pool);

#endif // THREAD_POOL_H
#pragma once
