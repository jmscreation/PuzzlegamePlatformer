#include "engine.h"

namespace Engine {

    FileResources::FileResources():
        success(true), failed(NULL) {}

    FileResources::FileResources(const std::vector<const char*>& array):
        success(true), failed(NULL) {
        load(array);
    }

    FileResources::FileResources(const std::vector<int>& array):
        success(true), failed(NULL) {
        load(array);
    }

    FileResources::~FileResources() {
        for(int i=0,l=package.size();i<l;i++)
            delete package[i];
    }

    void FileResources::load(const std::vector<const char*>& array) {
        for(int i=0,l=array.size();i<l;i++) {
            std::fstream file(array[i], std::ios::in | std::ios::binary);
            if(!file.is_open()){
                success = false;
                continue;
            }
            std::stringstream* f = new std::stringstream(std::ios::in | std::ios::out | std::ios::binary);
            *f << file.rdbuf();
            file.close();
            package.push_back(f);
        }
    }

    void FileResources::load(const std::vector<int>& array) {
        HMODULE curProc = GetModuleHandle(NULL);
        for(int i=0,l=array.size();i<l;i++) {
            std::stringstream* f = new std::stringstream(std::ios::in | std::ios::out | std::ios::binary);

            do {
                HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(array[i]),RT_RCDATA);
                if(!hres) {
                    success = false;
                    delete f;
                    f = NULL;
                    break;
                }

                HGLOBAL res = LoadResource(curProc,hres);
                if(!res) {
                    success = false;
                    delete f;
                    break;
                }

                const void* resdata = LockResource(res);
                size_t ressize = SizeofResource(curProc,hres);

                f->write((const char*)resdata, ressize);
                FreeResource(res);
            } while(0);

            package.push_back(f);
        }
    }

    bool FileResources::failureSubstitute(const char* path) {
        std::stringstream* f = new std::stringstream(std::ios::in | std::ios::out | std::ios::binary);
        if(failed != NULL)
            delete failed;
        std::fstream file(path, std::ios::in | std::ios::binary);
        *f << file.rdbuf();
        file.close();

        failed = f;
        return failed != NULL;
    }

    bool FileResources::failureSubstitute(int resource) {
        std::stringstream* f = new std::stringstream(std::ios::in | std::ios::out | std::ios::binary);

        if(failed != NULL)
            delete failed;

        HMODULE curProc = GetModuleHandle(NULL);
        do {
            HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(resource),RT_RCDATA);
            if(!hres) {
                success = false;
                delete f;
                f = NULL;
                break;
            }

            HGLOBAL res = LoadResource(curProc,hres);
            if(!res) {
                success = false;
                delete f;
                break;
            }

            const void* resdata = LockResource(res);
            size_t ressize = SizeofResource(curProc,hres);

            f->write((const char*)resdata, ressize);

            FreeResource(res);
        } while(0);

        failed = f;
        return failed != NULL;
    }

    bool FileResources::failureSubstitute(std::stringstream& str) {
        failed = new std::stringstream(std::ios::in | std::ios::out | std::ios::binary);
        *failed << str.rdbuf();
        return failed != NULL;
    }

    std::stringstream* FileResources::getFile(int ind) {
        if(ind < 0 || ind >= package.size() || package[ind] == NULL)
            return failed;
        return package[ind];
    }

    FontResources::FontResources():
        success(true), failed(NULL) {}

    FontResources::FontResources(const std::vector<const char*>& array):
        success(true), failed(NULL) {
        load(array);
    }

    FontResources::FontResources(const std::vector<int>& array):
        success(true), failed(NULL) {
        load(array);
    }

    FontResources::~FontResources() {
        for(int i=0,l=package.size();i<l;i++)
            delete package[i];
    }

    void FontResources::load(const std::vector<const char*>& array) {
        for(int i=0,l=array.size();i<l;i++) {
            sf::Font* fnt = new sf::Font();

            if(!fnt->loadFromFile(array[i])) {
                success = false;
                delete fnt;
                fnt = NULL;
            }

            package.push_back(fnt);
        }
    }

    void FontResources::load(const std::vector<int>& array) {
        HMODULE curProc = GetModuleHandle(NULL);
        for(int i=0,l=array.size();i<l;i++) {
            sf::Font* fnt = new sf::Font();

            do {
                HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(array[i]),RT_RCDATA);
                if(!hres) {
                    success = false;
                    delete fnt;
                    fnt = NULL;
                    break;
                }

                HGLOBAL res = LoadResource(curProc,hres);
                if(!res) {
                    success = false;
                    delete fnt;
                    break;
                }

                const void* resdata = LockResource(res);
                size_t ressize = SizeofResource(curProc,hres);

                if(!fnt->loadFromMemory(resdata,ressize)) {
                    success = false;
                    delete fnt;
                    fnt = NULL;
                }
                FreeResource(res);
            } while(0);

            package.push_back(fnt);
        }
    }

    bool FontResources::failureSubstitute(const char* path) {
        sf::Font* fnt = new sf::Font();

        if(failed != NULL)
            delete failed;

        if(!fnt->loadFromFile(path)) {
            success = false;
            delete fnt;
            fnt = NULL;
        }

        failed = fnt;
        return failed != NULL;
    }

    bool FontResources::failureSubstitute(int resource) {
        sf::Font* fnt = new sf::Font();

        if(failed != NULL)
            delete failed;

        HMODULE curProc = GetModuleHandle(NULL);
        do {
            HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(resource),RT_RCDATA);
            if(!hres) {
                success = false;
                delete fnt;
                fnt = NULL;
                break;
            }

            HGLOBAL res = LoadResource(curProc,hres);
            if(!res) {
                success = false;
                delete fnt;
                break;
            }

            const void* resdata = LockResource(res);
            size_t ressize = SizeofResource(curProc,hres);

            if(!fnt->loadFromMemory(resdata,ressize)) {
                success = false;
                delete fnt;
                fnt = NULL;
            }
            FreeResource(res);
        } while(0);

        failed = fnt;
        return failed != NULL;
    }

    bool FontResources::failureSubstitute(sf::Font& fnt) {
        failed = new sf::Font(fnt);
        return failed != NULL;
    }

    sf::Font* FontResources::getFont(int ind) {
        if(ind < 0 || ind >= package.size() || package[ind] == NULL)
            return failed;
        return package[ind];
    }

    ImageResources::ImageResources():
        success(true), failed(NULL) {}

    ImageResources::ImageResources(const std::vector<const char*>& array):
        success(true), failed(NULL) {
        load(array);
    }

    ImageResources::ImageResources(const std::vector<int>& array):
        success(true), failed(NULL) {
        load(array);
    }

    ImageResources::~ImageResources() {
        for(int i=0,l=package.size();i<l;i++)
            delete package[i];
    }

    void ImageResources::load(const std::vector<const char*>& array) {
        for(int i=0,l=array.size();i<l;i++) {
            sf::Image* tex = new sf::Image();

            if(!tex->loadFromFile(array[i])) {
                success = false;
                delete tex;
                tex = NULL;
            }

            package.push_back(tex);
        }
    }

    void ImageResources::load(const std::vector<int>& array) {
        HMODULE curProc = GetModuleHandle(NULL);
        for(int i=0,l=array.size();i<l;i++) {
            sf::Image* tex = new sf::Image();

            do {
                HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(array[i]),RT_RCDATA);
                if(!hres) {
                    success = false;
                    delete tex;
                    tex = NULL;
                    break;
                }

                HGLOBAL res = LoadResource(curProc,hres);
                if(!res) {
                    success = false;
                    delete tex;
                    break;
                }

                const void* resdata = LockResource(res);
                size_t ressize = SizeofResource(curProc,hres);

                if(!tex->loadFromMemory(resdata,ressize)) {
                    success = false;
                    delete tex;
                    tex = NULL;
                }
                FreeResource(res);
            } while(0);

            package.push_back(tex);
        }
    }

    bool ImageResources::failureSubstitute(const char* path) {
        sf::Image* tex = new sf::Image();

        if(failed != NULL)
            delete failed;

        if(!tex->loadFromFile(path)) {
            success = false;
            delete tex;
            tex = NULL;
        }

        failed = tex;
        return failed != NULL;
    }

    bool ImageResources::failureSubstitute(int resource) {
        sf::Image* tex = new sf::Image();

        if(failed != NULL)
            delete failed;

        HMODULE curProc = GetModuleHandle(NULL);
        do {
            HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(resource),RT_RCDATA);
            if(!hres) {
                success = false;
                delete tex;
                tex = NULL;
                break;
            }

            HGLOBAL res = LoadResource(curProc,hres);
            if(!res) {
                success = false;
                delete tex;
                break;
            }

            const void* resdata = LockResource(res);
            size_t ressize = SizeofResource(curProc,hres);

            if(!tex->loadFromMemory(resdata,ressize)) {
                success = false;
                delete tex;
                tex = NULL;
            }
            FreeResource(res);
        } while(0);

        failed = tex;
        return failed != NULL;
    }

    bool ImageResources::failureSubstitute(sf::Image& img) {
        failed = new sf::Image();
        failed->copy(img, 0, 0);
        return failed != NULL;
    }

    sf::Image* ImageResources::getImage(int ind) {
        if(ind < 0 || ind >= package.size() || package[ind] == NULL)
            return failed;
        return package[ind];
    }


    TextureResources::TextureResources():
        success(true), failed(NULL) {}

    TextureResources::TextureResources(const std::vector<const char*>& array):
        success(true), failed(NULL) {
        load(array);
    }

    TextureResources::TextureResources(const std::vector<int>& array):
        success(true), failed(NULL) {
        load(array);
    }

    TextureResources::~TextureResources() {
        for(int i=0,l=package.size();i<l;i++)
            delete package[i];
    }

    void TextureResources::load(const std::vector<const char*>& array) {
        for(int i=0,l=array.size();i<l;i++) {
            sf::Texture* tex = new sf::Texture();

            if(!tex->loadFromFile(array[i])) {
                success = false;
                delete tex;
                tex = NULL;
            }

            package.push_back(tex);
        }
    }

    void TextureResources::load(const std::vector<int>& array) {
        HMODULE curProc = GetModuleHandle(NULL);
        for(int i=0,l=array.size();i<l;i++) {
            sf::Texture* tex = new sf::Texture();

            do {
                HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(array[i]),RT_RCDATA);
                if(!hres) {
                    success = false;
                    delete tex;
                    tex = NULL;
                    break;
                }

                HGLOBAL res = LoadResource(curProc,hres);
                if(!res) {
                    success = false;
                    delete tex;
                    break;
                }

                const void* resdata = LockResource(res);
                size_t ressize = SizeofResource(curProc,hres);

                if(!tex->loadFromMemory(resdata,ressize)) {
                    success = false;
                    delete tex;
                    tex = NULL;
                }
                FreeResource(res);
            } while(0);

            package.push_back(tex);
        }
    }

    bool TextureResources::failureSubstitute(const char* path) {
        sf::Texture* tex = new sf::Texture();

        if(failed != NULL)
            delete failed;

        if(!tex->loadFromFile(path)) {
            success = false;
            delete tex;
            tex = NULL;
        }

        failed = tex;
        return failed != NULL;
    }

    bool TextureResources::failureSubstitute(int resource) {
        sf::Texture* tex = new sf::Texture();

        if(failed != NULL)
            delete failed;

        HMODULE curProc = GetModuleHandle(NULL);
        do {
            HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(resource),RT_RCDATA);
            if(!hres) {
                success = false;
                delete tex;
                tex = NULL;
                break;
            }

            HGLOBAL res = LoadResource(curProc,hres);
            if(!res) {
                success = false;
                delete tex;
                break;
            }

            const void* resdata = LockResource(res);
            size_t ressize = SizeofResource(curProc,hres);

            if(!tex->loadFromMemory(resdata,ressize)) {
                success = false;
                delete tex;
                tex = NULL;
            }
            FreeResource(res);
        } while(0);

        failed = tex;
        return failed != NULL;
    }

    bool TextureResources::failureSubstitute(sf::Image& img) {
        sf::Texture* tex = new sf::Texture();

        if(failed != NULL)
            delete failed;

        if(!tex->loadFromImage(img)) {
            success = false;
            delete tex;
            tex = NULL;
        }

        failed = tex;
        return failed != NULL;
    }

    sf::Texture* TextureResources::getTexture(int ind) {
        if(ind < 0 || ind >= package.size() || package[ind] == NULL)
            return failed;
        return package[ind];
    }


    AudioResources::AudioResources():
        success(true), failed(NULL) {}

    AudioResources::AudioResources(const std::vector<const char*>& array):
        success(true), failed(NULL) {
        load(array);
    }

    AudioResources::AudioResources(const std::vector<int>& array):
        success(true), failed(NULL) {
        load(array);
    }

    AudioResources::~AudioResources() {
        for(int i=0,l=package.size();i<l;i++)
            delete package[i];
    }

    void AudioResources::load(const std::vector<const char*>& array) {
        for(int i=0,l=array.size();i<l;i++) {
            SoundBuffer* snd = new SoundBuffer();

            if(!snd->loadOGGFromFile(array[i])) {
                success = false;
                delete snd;
                snd = NULL;
            }

            package.push_back(snd);
        }
    }

    void AudioResources::load(const std::vector<int>& array) {
        HMODULE curProc = GetModuleHandle(NULL);
        for(int i=0,l=array.size();i<l;i++) {
            SoundBuffer* snd = new SoundBuffer();

            do {
                HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(array[i]),RT_RCDATA);
                if(!hres) {
                    success = false;
                    delete snd;
                    snd = NULL;
                    break;
                }

                HGLOBAL res = LoadResource(curProc,hres);
                if(!res) {
                    success = false;
                    delete snd;
                    break;
                }

                const void* resdata = LockResource(res);
                size_t ressize = SizeofResource(curProc,hres);

                if(!snd->loadOGGFromMemory(resdata,ressize)) {
                    success = false;
                    delete snd;
                    snd = NULL;
                }
                FreeResource(res);
            } while(0);

            package.push_back(snd);
        }
    }

    bool AudioResources::failureSubstitute(const char* path) {
        SoundBuffer* snd = new SoundBuffer();

        if(failed != NULL)
            delete failed;

        if(!snd->loadOGGFromFile(path)) {
            success = false;
            delete snd;
            snd = NULL;
        }

        failed = snd;
        return failed != NULL;
    }

    bool AudioResources::failureSubstitute(int resource) {
        SoundBuffer* snd = new SoundBuffer();

        if(failed != NULL)
            delete failed;

        HMODULE curProc = GetModuleHandle(NULL);
        do {
            HRSRC hres = FindResource(curProc,MAKEINTRESOURCE(resource),RT_RCDATA);
            if(!hres) {
                success = false;
                delete snd;
                snd = NULL;
                break;
            }

            HGLOBAL res = LoadResource(curProc,hres);
            if(!res) {
                success = false;
                delete snd;
                break;
            }

            const void* resdata = LockResource(res);
            size_t ressize = SizeofResource(curProc,hres);

            if(!snd->loadOGGFromMemory(resdata,ressize)) {
                success = false;
                delete snd;
                snd = NULL;
            }
            FreeResource(res);
        } while(0);

        failed = snd;
        return failed != NULL;
    }

    SoundBuffer* AudioResources::getSound(int ind) {
        if(ind < 0 || ind >= package.size() || package[ind] == NULL)
            return failed;
        return package[ind];
    }

}
