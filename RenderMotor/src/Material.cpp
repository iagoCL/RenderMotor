#include "Material.h"

Material::Material(IlluminationSet* illuminationSet_)
    : illuminationSet(illuminationSet_) {
    illuminationSet_->addMaterial(this);
}
IlluminationSet* Material::getIlluminationSet() const {
    return illuminationSet;
}

void Material::addNode(Node* node) {
    nodes.push_back(node);
}
void Material::renderNodes(glm::mat4 view, glm::mat4 proj) const {
    activateMaterial(view);
    for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
        (*nodeIt)->renderNode(view, proj);
    }
}
SimpleMaterial::SimpleMaterial(IlluminationSet* illuminationSet_)
    : Material(illuminationSet_) {
}
void SimpleMaterial::activateMaterial(glm::mat4 view) const {
}
BumpMaterial::BumpMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_, Texture* emissiveText_, Texture* specularText_, Texture* normalsText_)
    : Material(illuminationSet_),
      diffuseText(diffuseText_),
      emissiveText(emissiveText_),
      specularText(specularText_),
      normalsText(normalsText_) {
    unsigned int program = illuminationSet_->getShader()->getProgram();

    uColorTex = glGetUniformLocation(program, "colorTex");
    uEmiTex = glGetUniformLocation(program, "emiTex");
    uSpecTex = glGetUniformLocation(program, "specularTex");
    uNormTex = glGetUniformLocation(program, "normalTex");
}

void BumpMaterial::activateMaterial(glm::mat4 view) const {
    if (uColorTex != -1 && diffuseText) {
        diffuseText->sendToShaderProgram(uColorTex);
    }
    if (uEmiTex != -1 && emissiveText) {
        emissiveText->sendToShaderProgram(uEmiTex);
    }
    if (uSpecTex != -1 && specularText) {
        specularText->sendToShaderProgram(uSpecTex);
    }
    if (uNormTex != -1 && normalsText) {
        normalsText->sendToShaderProgram(uNormTex);
    }
}