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
int MIN_DEPTH;
int has_depth(const char *path, int current_depth, int min_depth) {
    if (current_depth >= min_depth) {
        return 1;
    }

    struct stat info;
    if (lstat(path, &info) == -1) {
        return 0;
    }

    if (!S_ISDIR(info.st_mode)) {
        return 0;
    }

    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, NULL);
    if (n < 0) return 0;

    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(namelist[i]->d_name, ".") == 0 || strcmp(namelist[i]->d_name, "..") == 0) {
            free(namelist[i]);
            continue;
        }
        
        if (!found) {
            char new_path[MAX_PATH];
            snprintf(new_path, MAX_PATH, "%s/%s", path, namelist[i]->d_name);
            if (has_depth(new_path, current_depth + 1, MIN_DEPTH)) {
                found = 1;
            }
        }
        free(namelist[i]);
    }
    free(namelist);
    return found;
}

void print_dir(const char *path, const char *prefix, int last, int current_depth)  
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
            snprintf(new_path, MAX_PATH, "%s/%s", path, namelist[i]->d_name);
            if (has_depth(new_path, current_depth + 1, MIN_DEPTH)) {
                count++;
            }
        }
    }

    int current_idx = 0;
    for (int i = 0; i < n; i++) {
        struct dirent *entry = namelist[i];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            free(entry);
            continue;
        }

        snprintf(new_path, MAX_PATH, "%s/%s", path, entry->d_name);
        
        if (!has_depth(new_path, current_depth + 1, MIN_DEPTH)) {
            free(entry);
            continue;
        }

        current_idx++;
        int is_last_child = (current_idx == count);

        if(lstat(new_path, &info) == -1)
        {
        }
        
        if(S_ISDIR(info.st_mode))
        {
            print_dir(new_path, new_prefix, is_last_child, current_depth + 1);
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
    if(argc!=2 && argc!=3)
    {
        fprintf(stderr, "Usage: %s <directory> <min_depth>\n", argv[0]);
        return 1;
    }
    
    if(argc==3)
    {
        MIN_DEPTH = atoi(argv[2]);
    }

    if (has_depth(argv[1], 0, MIN_DEPTH)) {
        print_dir(argv[1], "", 1, 0);
    }
    else
    {
        fprintf(stderr, "Directory %s does not have a depth of %d\n", argv[1], MIN_DEPTH);
        return 1;
    }
    if(argc==2)
    {
        print_dir(argv[1], "", 1, 0);
    }
    return 0;
}