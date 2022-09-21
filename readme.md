# 路径追踪实践——smallpt源码解读

（请来自知乎的朋友查看这里获取实例代码 https://github.com/Zydiii/PathTracing/tree/c5f8b85cc4817853ec25af20b06ef40dbefbef38 ，因为当前最新的提交中有很多更改的内容没有写到文章里）

https://zhuanlan.zhihu.com/p/490551718

采样率 4

![](./result/result_4.png)

采样率 40

![](./result/result_40.png)


采样率 400

![](./result/result_400.png)

采样率 4000

![](./result/result_4000.png)

采样率 20000

![](./result/result_20000.png)

## ISSUE

在 Debug 模式下性能很低，渲染进度可能长时间都没有变化，可以尝试减少采样率，只在 Release 模式下运行高采样率
