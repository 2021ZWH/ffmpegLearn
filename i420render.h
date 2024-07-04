#ifndef I420RENDER_H
#define I420RENDER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLWidget>


class I420Render : public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:
    I420Render(QWidget *parent =nullptr);
    ~I420Render();
    void init(int width,int height);
    void initializeGL() override;
    void resizeGL(int w,int h) override;
    void paintGL() override;
public slots:
    void updateShow(unsigned char *buff);
private:
    //shader程序
    QOpenGLShaderProgram m_program;
    //shader中yuv变量地址
    GLuint m_textureUniformY, m_textureUniformU , m_textureUniformV;
    //创建纹理
    GLuint m_idy , m_idu , m_idv;

    unsigned char *moutBuff=NULL;
    int width=0,height=0;


};

#endif // I420RENDER_H
