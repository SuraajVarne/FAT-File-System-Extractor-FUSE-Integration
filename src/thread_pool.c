#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread_pool.h"

typedef struct task_node {
    task_t task;
    struct task_node* next;
} task_node_t;

struct thread_pool {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t* threads;
    task_node_t* task_head;
    task_node_t* task_tail;
    size_t thread_count;
    int shutdown;
};

static void* worker_thread(void* arg) {
    thread_pool_t* pool = (thread_pool_t*)arg;
    while (1) {
        pthread_mutex_lock(&pool->lock);
        while (!pool->task_head && !pool->shutdown) {
            pthread_cond_wait(&pool->cond, &pool->lock);
        }
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }
        task_node_t* node = pool->task_head;
        if (node) {
            pool->task_head = node->next;
            if (!pool->task_head) {
                pool->task_tail = NULL;
            }
        }
        pthread_mutex_unlock(&pool->lock);
        if (node) {
            node->task.func(node->task.arg);
            free(node);
        }
    }
    return NULL;
}

thread_pool_t* thread_pool_create(size_t num_threads) {
    thread_pool_t* pool = malloc(sizeof(thread_pool_t));
    if (!pool) return NULL;
    pool->thread_count = num_threads;
    pool->shutdown = 0;
    pool->task_head = pool->task_tail = NULL;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->threads = malloc(num_threads * sizeof(pthread_t));
    if (!pool->threads) {
        free(pool);
        return NULL;
    }
    for (size_t i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    }
    return pool;
}

void thread_pool_enqueue(thread_pool_t* pool, task_func func, void* arg) {
    task_node_t* node = malloc(sizeof(task_node_t));
    if (!node) return;
    node->task.func = func;
    node->task.arg = arg;
    node->next = NULL;
    pthread_mutex_lock(&pool->lock);
    if (pool->task_tail) {
        pool->task_tail->next = node;
        pool->task_tail = node;
    }
    else {
        pool->task_head = pool->task_tail = node;
    }
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->lock);
}

void thread_pool_destroy(thread_pool_t* pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->lock);
    for (size_t i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    // Free remaining tasks.
    while (pool->task_head) {
        task_node_t* temp = pool->task_head;
        pool->task_head = pool->task_head->next;
        free(temp);
    }
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    free(pool);
}
