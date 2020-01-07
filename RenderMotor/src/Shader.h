#ifndef RENDER_MOTOR_SHADER_PROGRAM_H
#define RENDER_MOTOR_SHADER_PROGRAM_H

class Shader {
    //Definimos las variables que nos dan acceso a los shaders

   private:
    unsigned int vshader;
    unsigned int fshader;
    unsigned int program;

    //Variables con las que hacedemos a los atributos de los shaders
    //Variables Uniform
    int uModelViewMat;
    int uModelViewProjMat;
    int uNormalMat;

    //uniformes luz
    int* uLPos;
    int* uLDir;
    int* uLCol;
    int* uLAngle;

    //Texturas Uniform
    int uColorTex;
    int uEmiTex;
    int uSpecTex;
    int uNormTex;

    //Atributos
    int inPos;
    int inColor;
    int inNormal;
    int inTexCoord;
    int inTangent;

    int LucesActuales;

   public:
    Shader(const char* vname, const char* fname, int numeroDeLuces = 3);
    ~Shader();

    int loadShader(const char* fileName, int type);
    void initShader(const char* vname, const char* fname, int numeroDeLuces);

    unsigned int getprogram();
    int getuModelViewMat();
    int getuModelViewProjMat();
    int getuNormalMat();

    int getuLPos(int numero = 0);
    int getuLCol(int numero = 0);
    int getuLDir(int numero = 0);
    int getuLAngle(int numero = 0);

    int getuColorTex();
    int getuNormTex();
    int getuEmiTex();
    int getuSpecTex();
    int getinPos();
    int getinColor();
    int getinNormal();
    int getinTexCoord();
    int getinTangent();
    int getProgram();
};
#endif //RENDER_MOTOR_SHADER_PROGRAM_H