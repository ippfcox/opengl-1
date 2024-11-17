# note

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