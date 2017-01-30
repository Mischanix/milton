#include "platform.h"

#include "common.h"
#include "memory.h"

#define __USE_MISC 1  // MAP_ANONYMOUS and MAP_NORESERVE dont' get defined without this
#include <sys/mman.h>
#undef __USE_MISC

#include <android/log.h>

#define IMPL_MISSING mlt_assert(!"IMPLEMENT")


float
perf_count_to_sec(u64 counter)
{
    // Input as nanoseconds
    return (float)counter * 1e-9;
}

u64
perf_counter()
{
    // http://stackoverflow.com/a/2660610/4717805
    timespec tp;
    int res = clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

    // TODO: Check errno and provide more informations
    if ( res ) {
        milton_log("Something went wrong with clock_gettime\n");
    }

    return tp.tv_nsec;
}

b32
platform_delete_file_at_config(PATH_CHAR* fname, int error_tolerance)
{
    char fname_at_config[MAX_PATH];
    strncpy(fname_at_config, fname, MAX_PATH);
    platform_fname_at_config(fname_at_config, MAX_PATH*sizeof(char));
    int res = remove(fname_at_config);
    b32 result = true;
    if ( res != 0 ) {
        result = false;
        // Delete failed for some reason.
        if ( (error_tolerance == DeleteErrorTolerance_OK_NOT_EXIST) &&
             (errno == EEXIST || errno == ENOENT) ) {
            result = true;
        }
    }

    return result;
}

void
platform_dialog(char* info, char* title)
{
    // IMPL_MISSING;
    return;
}

b32
platform_dialog_yesno(char* info, char* title)
{
    // IMPL_MISSING;
    return false;
}

void
platform_fname_at_config(PATH_CHAR* fname, size_t len)
{
    // TODO: remove this allocation
    char *string_copy = (char*)mlt_calloc(1, len, "Strings");
    if ( string_copy ) {
        strncpy(string_copy, fname, len);
        char *folder;
        char *home = getenv("XDG_DATA_HOME");
        if ( !home ) {
            home = getenv("HOME");
            folder = ".milton";
        } else {
            folder = "milton";
        }
        snprintf(fname, len, "%s/%s", home, folder);
        mkdir(fname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        strncat(fname, "/", len);
        strncat(fname, string_copy, len);
        mlt_free(string_copy, "Strings");
    }
}

void
platform_fname_at_exe(PATH_CHAR* fname, size_t len)
{
    /* just use relative path to cwd for this?
    u32 bufsize = (u32)len;
    char buffer[MAX_PATH] = {0};
    strncpy(buffer, fname, MAX_PATH);
    strncpy(fname, program_invocation_name, bufsize);
    {  // Remove the executable name
        PATH_CHAR* last_slash = fname;
        for( PATH_CHAR* iter = fname;
            *iter != '\0';
            ++iter ) {
            if ( *iter == '/' ) {
                last_slash = iter;
            }
        }
        *(last_slash+1) = '\0';
    }
    strncat(fname, "/", len);
    strncat(fname, buffer, len);
    */
    return;
}

FILE*
platform_fopen(const PATH_CHAR* fname, const PATH_CHAR* mode)
{
    FILE* fd = fopen_unix(fname, mode);
    return fd;
}

b32
platform_move_file(PATH_CHAR* src, PATH_CHAR* dest)
{
    int res = rename(src, dest);

    return res == 0;
}

PATH_CHAR*
platform_open_dialog(FileKind kind)
{
    return NULL;
/*
    platform_cursor_show();
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
            "Open File",
            NULL,
            GTK_FILE_CHOOSER_ACTION_OPEN,
            "Cancel", GTK_RESPONSE_CANCEL,
            "Open", GTK_RESPONSE_ACCEPT,
            NULL
            );
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = gtk_file_filter_new();
    linux_set_GTK_filter(chooser, filter, kind);
    gtk_file_chooser_set_current_folder(chooser, getenv("HOME"));
    if ( gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_ACCEPT ) {
        gtk_widget_destroy(dialog);
        return NULL;
    }
    gchar *gtk_filename = gtk_file_chooser_get_filename(chooser);
    PATH_CHAR* open_filename = (PATH_CHAR*)mlt_calloc(MAX_PATH, sizeof(PATH_CHAR), "Strings");

    PATH_STRNCPY(open_filename, gtk_filename, MAX_PATH);
    g_free(gtk_filename);
    gtk_widget_destroy(dialog);
    return open_filename;
*/
}

void
platform_open_link(char* link)
{
    IMPL_MISSING;
    return;
}

PATH_CHAR*
platform_save_dialog(FileKind kind)
{
    return NULL;
    /*
    platform_cursor_show();
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
            "Save File",
            NULL,
            GTK_FILE_CHOOSER_ACTION_SAVE,
            "Cancel", GTK_RESPONSE_CANCEL,
            "Save", GTK_RESPONSE_ACCEPT,
            NULL
            );
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = gtk_file_filter_new();
    linux_set_GTK_filter(chooser, filter, kind);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    gtk_file_chooser_set_current_folder(chooser, getenv("HOME"));
    gtk_file_chooser_set_current_name(chooser, kind == FileKind_IMAGE ? "untitled.jpg" : "untitled.mlt");
    if ( gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_ACCEPT ) {
        gtk_widget_destroy(dialog);
        return NULL;
    }
    gchar *gtk_filename = gtk_file_chooser_get_filename(chooser);
    PATH_CHAR* save_filename = (PATH_CHAR*)mlt_calloc(1, MAX_PATH*sizeof(PATH_CHAR), "Strings");

    PATH_STRNCPY(save_filename, gtk_filename, MAX_PATH);
    g_free(gtk_filename);
    gtk_widget_destroy(dialog);
    return save_filename;
    */
}
//  ====

WallTime
platform_get_walltime()
{
    WallTime wt = {0};
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *time;
    time = localtime(&tv.tv_sec);
    wt.h = time->tm_hour;
    wt.m = time->tm_min;
    wt.s = time->tm_sec;
    wt.ms = tv.tv_usec / 1000;
    return wt;
}

FILE*
fopen_unix(const char* fname, const char* mode)
{
    FILE* fd = fopen(fname, mode);
    return fd;
}

void
milton_fatal(char* message)
{
    milton_log("*** [FATAL] ***: \n\t");
    puts(message);
    __builtin_trap();
}

// TODO: Show a message box, and then die
void
milton_die_gracefully(char* message)
{
    milton_log("*** [FATAL] ***: \n\t");
    milton_log(message);
    __builtin_trap();
}

void platform_load_gl_func_pointers() {}

typedef struct UnixMemoryHeader_s
{
    size_t size;
} UnixMemoryHeader;

void*
platform_allocate(size_t size)
{
    u8* ptr = (u8*)mmap(NULL, size + sizeof(UnixMemoryHeader),
                        PROT_WRITE | PROT_READ,
                        /*MAP_NORESERVE |*/ MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);
    if ( ptr ) {
        // NOTE: This should be a footer if we intend on returning aligned data.
        *((UnixMemoryHeader*)ptr) = (UnixMemoryHeader)
        {
            .size = size,
        };
        ptr += sizeof(UnixMemoryHeader);
    }
    return ptr;
}

void
platform_deallocate_internal(void* ptr)
{
    mlt_assert(ptr);
    u8* begin = (u8*)ptr - sizeof(UnixMemoryHeader);
    size_t size = *((size_t*)begin);
    munmap(ptr, size);
}

// TODO: haven't checked if platform_cursor_hide or platform_cursor_show work.
void
platform_cursor_hide()
{
    int lvl = SDL_ShowCursor(-1);
    if ( lvl >= 0 ) {
        mlt_assert ( lvl == 1 );
        int res = SDL_ShowCursor(0);
        if ( res < 0 ) {
            INVALID_CODE_PATH;
        }
    }
}

void
platform_cursor_show()
{
    int lvl = SDL_ShowCursor(-1);
    if ( lvl < 0 )
    {
        mlt_assert ( lvl == -1 );
        SDL_ShowCursor(1);
    }
}

i32
platform_monitor_refresh_hz()
{
    i32 hz = 60;
    // TODO: Implement this on macOs and Linux.
    return hz;
}

int
main(int argc, char** argv)
{
    return milton_main();
}

int milton_log_adb(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = __android_log_vprint(ANDROID_LOG_INFO, "Milton", format, args);
    va_end(args);
    return result;
}
