#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rest_api.h"
#include "fat_parser.h"
#include "utils.h"

#define RESPONSE_BUFFER_SIZE 4096

static int api_handler(void* cls, struct MHD_Connection* connection,
    const char* url, const char* method,
    const char* version, const char* upload_data,
    size_t* upload_data_size, void** con_cls) {
    (void)cls; (void)url; (void)version; (void)upload_data; (void)upload_data_size; (void)con_cls;
    const char* json_response = "{\"status\": \"FAT metadata not implemented\"}";
    struct MHD_Response* response = MHD_create_response_from_buffer(strlen(json_response), (void*)json_response, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int start_rest_api_server(int port, const char* image_path) {
    // For this demo, we ignore image_path; in a complete version, you would load and parse the FAT image.
    struct MHD_Daemon* daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port, NULL, NULL, &api_handler, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        fprintf(stderr, "Failed to start REST API server.\n");
        return -1;
    }
    printf("REST API server running on port %d. Press Enter to stop.\n", port);
    getchar();
    MHD_stop_daemon(daemon);
    return 0;
}
