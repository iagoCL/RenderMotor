#include "IlluminationSet.h"

IlluminationSet::IlluminationSet(Shader* shader_)
    : shader(shader_) {
    shader->addIllumination(this);
}
Shader* IlluminationSet::getShader() const {
    return shader;
}
void IlluminationSet::sendIlluminationToShader(glm::mat4 view) const {
    for (auto lightIt = lights.begin(); lightIt != lights.end(); ++lightIt) {
        lightIt->light->sendToShader(view, lightIt->uLPos, lightIt->uLDir, lightIt->uLAngle);
        lightIt->light->sendColorToShader(lightIt->uLCol);
    }
}
void IlluminationSet::renderMaterials(glm::mat4 view) const {
    sendIlluminationToShader(view);
    for (auto materialIt = materials.begin(); materialIt != materials.end(); ++materialIt) {
        (*materialIt)->renderNodes(view);
    }
}
void IlluminationSet::addLight(Light* light) {
    std::string lightNumber = std::to_string(lights.size() + 1);
    int program = shader->getProgram();
    std::string aux = std::string("Posl") + lightNumber;
    GLint uLPos = glGetUniformLocation(program, &aux[0]);
    aux = std::string("Dirl") + lightNumber;
    GLint uLDir = glGetUniformLocation(program, &aux[0]);
    aux = std::string("Il") + lightNumber;
    GLint uLCol = glGetUniformLocation(program, &aux[0]);
    aux = std::string("Anglel") + lightNumber;
    GLint uLAngle = glGetUniformLocation(program, &aux[0]);
    lights.emplace_back(light, uLPos, uLDir, uLCol, uLAngle);
}
void IlluminationSet::addMaterial(Material* material) {
    materials.push_back(material);
}