class Texture {
    private:
        unsigned int textureId;
        unsigned int textureUnit;
    public:
        Texture(char const *path, unsigned int unit);
        unsigned int GetTextureId() const { return textureId; }
        unsigned int GetTextureUnit() const { return textureUnit; }
    protected:
        int LoadTexture(char const *path) const;
};