### todo ...

```c++
#include <stdio.h>
#include <string.h>

struct login_private_data {
    int status;
    const char *token;  
};

void login_callback(void *data, int status, const char *token) {
    struct login_private_data *private_data = (struct login_private_data *) data;
    private_data->status = status;
    private_data->token = strdup(token);
}

int main(int, char **) {
    struct login_private_data private_data = {0};

    int err = stigmark_client_login("zexigh@gmail.com", "foobar", login_callback, &private_data, 1);
    if (err < 0) {
        fprintf(stderr, "login: error %d", err);
        return -1;
    }

    if (private_data.status >= 200 && private_data.status < 300) {
        printf("login: token '%s'\n", private_data.token);
        return 0;
    }

    fprintf(stderr, "login: failed with status=%d", private_data.status);
    return -1;
}
```

