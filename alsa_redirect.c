#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

static int (*wrap_snd_pcm_open)(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode) = NULL;

void * wrap(void * func,char *name)
{
    char *msg;
    if (func == NULL) {
        func = dlsym(RTLD_NEXT, name);
        fprintf(stderr, "** wrapping %s =>  %p\n", name,func);
        if ((msg = dlerror()) != NULL) {
            fprintf(stderr, "** %s: dlopen failed : %s\n", name,msg);
            exit(1);
        } else {
            fprintf(stderr, "** %s: wrapping done\n",name);
        }
    }
    return func;
}

int snd_pcm_open(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode)
{
    int temp;

    char* env_name = getenv("ALSAREDIRECT_PCM");
    wrap_snd_pcm_open = wrap(wrap_snd_pcm_open,"snd_pcm_open");
    temp = wrap_snd_pcm_open(pcm,env_name,stream,mode);
    fprintf(stderr, "** Calling snd_pcm_open for path:[%s] => fd:[%d] instead of path:[%s]\n",env_name,temp, name);
    fflush(stderr);
    return temp;

}
