
#include "custom_listdir.h"

#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*

Based on http://thp.io/2012/archive/qdir_entrylist_performance.cpp
Thomas Perl <m@thp.io>, 2012-08-09

*/

struct file_entry {
    char *name;
    time_t mtime;
};

int
compare_file_entry(const void *a, const void *b)
{
    struct file_entry *ea = (struct file_entry*)a;
    struct file_entry *eb = (struct file_entry*)b;

    return eb->mtime - ea->mtime;
}

/* Tweakable values for the pre-allocated list size and its growth */
#define LISTDIR_INITIAL_SIZE 1024
#define LISTDIR_STEP_FACTOR 2

#define str_endswith_ignorecase(s, ext) \
    (strlen(s) >= strlen(ext) && \
     strcasecmp((s) + strlen(s) - strlen(ext), (ext)) == 0)

QStringList
custom_listdir(QDir &directory)
{
    char *dirname = strdup((directory.absolutePath()+"/")
        .toLocal8Bit().constData());

    struct dirent *ent;
    struct stat st;
    char name[PATH_MAX];

    struct file_entry *entry = (struct file_entry*)
        calloc(LISTDIR_INITIAL_SIZE, sizeof(struct file_entry));
    int size = LISTDIR_INITIAL_SIZE;
    int count = 0;

    assert(entry != NULL);

    DIR *dir = opendir(dirname);
    assert(dir != NULL);
    while ((ent = readdir(dir)) != NULL) {
        if (!str_endswith_ignorecase(ent->d_name, ".jpg")) {
            continue;
        }

        strncpy(name, dirname, sizeof(name)-1);
        strncat(name, ent->d_name, sizeof(name)-1);
        assert(stat(name, &st) == 0);

        if (size < count + 1) {
            size *= LISTDIR_STEP_FACTOR;
            entry = (struct file_entry*)
                realloc(entry, sizeof(struct file_entry)*size);
            assert(entry != NULL);
        }

        entry[count].name = strdup(ent->d_name);
        entry[count].mtime = st.st_mtime;

        count++;
    }
    closedir(dir);

    qsort(entry, count, sizeof(struct file_entry), compare_file_entry);

    QStringList result;

    for (int i=0; i<count; i++) {
        result << entry[i].name;
        free(entry[i].name);
    }

    free(entry);
    free(dirname);

    return result;
}

