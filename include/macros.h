#if !defined(MACROS_H)

/* detect whether __VA_OPT__ supported 
   credit: https://stackoverflow.com/a/48045656
*/
#define THIRD_ARG(a,b,c,...) c
#define VA_OPT_AVAIL_I(...) THIRD_ARG(__VA_OPT__(,),1,0,)
#define VA_OPT_AVAIL VA_OPT_AVAIL_I(?)

/* make variadic macros agnostic of __VA_OPT__ support */
#if VA_OPT_AVAIL
  #define PASS_ARGS_I(...) __VA_OPT__(,) __VA_ARGS__
#else
  #define PASS_ARGS_I(...) , ##__VA_ARGS__
#endif

#define PASS_ARGS(...) PASS_ARGS_I(__VA_ARGS__)

#define LOG_MESSAGE(fmt, ...)                                                    \
    fprintf(stderr, "OMPT: [%-36s] " fmt "\n", __func__ PASS_ARGS(__VA_ARGS__))

#define SET_CALLBACK(event)                                                   \
do {                                                                          \
    result = ompt_set_callback(event, (ompt_callback_t) on_##event);          \
    fprintf(stderr, "OMPT: set callback %-36s = %d\n", "on_" #event, result); \
} while(0);

#endif // MACROS_H
