#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include <QOpenGLTexture>
#include <string>
class TextureManager
{
public:
    TextureManager();

    static void init();
    static std::map<std::string, QOpenGLTexture*> m_textures;
    static QOpenGLTexture* getTexture(std::string name);
};

#endif // TEXTUREMANAGER_H
