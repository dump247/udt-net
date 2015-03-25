# Introduction #

Getting performance metrics from a socket is pretty straight forward.

```
Udt.TraceInfo perf = socket.GetPerformanceInfo();
```

There are three classes of performance metrics: Total, Local, and Probe. Total metrics are aggregate values calculated since the socket was created. Local metrics are aggregate values calculated since they were recorded. Local metrics can be reset by passing true to **GetPerformanceInfo(bool)**. The local values are then reset for the next call to GetPerformanceInfo. Probe values are instant values collected at the time that GetPerformanceInfo is called.