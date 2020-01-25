#include "IlluminationSet.h"

unsigned int IlluminationSet::numIlluminationsSets = 0;

std::shared_ptr<IlluminationSet> IlluminationSet::createIlluminationSet(std::shared_ptr<Shader> shader_) {
    std::shared_ptr<IlluminationSet> illuminationSet(new IlluminationSet(shader_));
    shader_->addIllumination(illuminationSet);
    return illuminationSet;
}
IlluminationSet::IlluminationSet(std::shared_ptr<Shader> shader_)
    : shader(shader_),
      id(numIlluminationsSets++) {
    std::cout << "Created IlluminationSet: " << id << std::endl;
}
IlluminationSet::~IlluminationSet() {
    std::cout << "Created IlluminationSet: " << id << std::endl;
}
std::shared_ptr<Shader> IlluminationSet::getShader() const {
    return shader;
}
void IlluminationSet::sendIlluminationToShader(const glm::mat4& view) const {
    for (auto lightIt = lights.begin(); lightIt != lights.end(); ++lightIt) {
        lightIt->light->sendToShader(view, lightIt->uLPos, lightIt->uLDir, lightIt->uLAngle);
        lightIt->light->sendColorToShader(lightIt->uLCol);
    }
}
void IlluminationSet::renderMaterials(const glm::mat4& view) const {
    sendIlluminationToShader(view);
    for (auto materialIt = materials.begin(); materialIt != materials.end(); ++materialIt) {
        (*materialIt)->renderNodes(view);
    }
}
void IlluminationSet::addLight(std::shared_ptr<Light> light) {
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
void IlluminationSet::addMaterial(std::shared_ptr<Material> material) {
    materials.push_back(material);
}