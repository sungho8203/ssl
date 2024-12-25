#ifndef TYPE_H
#define TYPE_H

enum class StatusCode {
    OK = 200,
    CREATED = 201,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500
};

enum class HTTP_ErrorCodes{
    requestErrorCode = 100,
    FAIL_SetRequest = 101,

    responseErrorCode = 200,
    FAIL_SetStatus = 201,
    FAIL_SetContentType = 202,
    FAIL_SetBody = 203,
};

#endif