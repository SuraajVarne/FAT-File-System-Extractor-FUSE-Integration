#ifndef REST_API_H
#define REST_API_H

// Start the RESTful API server on the given port.
// The server will expose FAT metadata as JSON.
int start_rest_api_server(int port, const char* image_path);

#endif // REST_API_H
#pragma once
