#ifdef WATADDON_EXPORTS
#define WATAPI __attribute__ ((visibility("default")))
#endif



#ifdef __cplusplus
extern "C" {
#endif

    int WATAPI doStuff()
    {
        return 3;
    }

#ifdef __cplusplus
}
#endif
