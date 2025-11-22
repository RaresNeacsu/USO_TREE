#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_PATH 1024
#define MAX_PREFIX 1024
#define MAX_NAME 256

void print_dir(const char *path, const char *prefix, int last)  
{
    struct dirent **namelist;
    struct stat info;
    char new_path[MAX_PATH];
    char new_prefix[MAX_PREFIX];
    int n;

    const char *name = strchr(path, '/');
    if(name) name++; else name = path;

    if (prefix[0] != '\0' || last){
         if(last)
        {
            printf("%s└── ", prefix);
            snprintf(new_prefix, MAX_PREFIX, "%s    ", prefix);
        }
        else
        {
            printf("%s├── ", prefix);
            snprintf(new_prefix, MAX_PREFIX, "%s│   ", prefix);
        }
        printf("%s\n", name);
    } else {
        printf("%s\n", path);
        snprintf(new_prefix, MAX_PREFIX, "%s", "");
    }
    
    n = scandir(path, &namelist, NULL, alphasort);
    if (n < 0) {
        perror("scandir");
        return;
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0) {
            count++;
        }
    }

    int current_idx = 0;
    for (int i = 0; i < n; i++) {
        struct dirent *entry = namelist[i];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            free(entry);
            continue;
        }

        current_idx++;
        int is_last_child = (current_idx == count);

        snprintf(new_path, MAX_PATH, "%s/%s", path, entry->d_name);
        if(lstat(new_path, &info) == -1)
        {
        }
        
        if(S_ISDIR(info.st_mode))
        {
            print_dir(new_path, new_prefix, is_last_child);
        }
        else
        {
            if(is_last_child)
                printf("%s└── %s\n", new_prefix, entry->d_name);
            else
                printf("%s├── %s\n", new_prefix, entry->d_name);
        }
        free(entry);
    }
    free(namelist);
}

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }
    print_dir(argv[1], "", 1);
    return 0;
}