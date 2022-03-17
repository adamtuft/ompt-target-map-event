This repository reproduces behaviour observed when trying to use the OMPT interface to trace *target-map* events in the OpenMP runtime.

The OpenMP tool `tool.c` registers a single callback which should be triggered upon the *target-map* runtime event. The `matmul` example program performs GPU offloading with OpenMP which includes such an event. It is expected that the callback registered by the OpenMP tool should trigger, however this is not observed.

The OpenMP runtime reports that the callback is registered successfully and is always invoked (according to the result returned by `ompt_set_callback`).

To submit on devcloud:

```
./submit.sh job.sh
```

Expected results:

```
$ grep "^OMPT" err.txt 
OMPT: Runtime: Intel(R) OMP version: 5.0.20211013
OMPT: Set callback for ompt_callback_target_map event with result: 5
OMPT: on_ompt_callback_target_map
...
OMPT: Finalising tool
```

Observed results:

```
$ grep "^OMPT" err.txt 
OMPT: Runtime: Intel(R) OMP version: 5.0.20211013
OMPT: Set callback for ompt_callback_target_map event with result: 5
OMPT: Finalising tool
```