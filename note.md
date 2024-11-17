# note

## 整体的流程

1. 准备顶点数据，创建vbo，可能包含了顶点的坐标、颜色、法线、uv等
2. 准备索引数据，创建ebo
3. 创建vao，对vbo中的各数据进行描述，0号为坐标、1号为颜色等，以及各数据的访问方式
4. 绑定ebo，这里很关键，必须在绑定vao之后绑定的ebo才能与vao联系起来
5. 准备顶点着色器和片段着色器代码，编译，链接，并检查错误
6. 绑定着色器程序，绑定vao
7. 绘制
8. 解绑

## VBO与VAO的一般创建方式

首先我们认为有这样的vertices：

```c
    float vertices[] = {
    //  x   , y   , z   , r   , g   , b   , a   , u   , v
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
```

创建vbo并上传数据

```c
    GLuint vbo;
    // 创建，此时有结构，但没有显存空间
    glGenBuffers(1, &vbo);
    // 绑定到GL_ARRAY_BUFFER这个目标上
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 上传数据到绑定的GL_ARRAY_BUFFER，也就是vbo上，这里会申请显存空间
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
```

处理vao

```c
    GLuint vao;
    // 创建并绑定vao，在这后面的都是向此vao操作
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // 绑定vbo，后面的都向此vbo操作
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 激活0号描述
    glEnableVertexAttribArray(0);
    // 配置位置信息的描述
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(7 * sizeof(float)));
    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
```

## glDrawElements

关于indices这个参数

- 当使用ebo时，最后的参数为偏移量
- 当未使用ebo时，最后的参数为传入的indices指针

如果绑定了ebo，最后还是传入指针，那么该指针会被当成偏移量，一般来说，都会什么都绘制不出来（因为偏移很容易就过大，超出indices的长度）