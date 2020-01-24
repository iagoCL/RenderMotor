#include "Material.h"

Material::Material(IlluminationSet* illuminationSet_)
    : illuminationSet(illuminationSet_) {
    illuminationSet_->addMaterial(this);
}
SimpleMaterial::SimpleMaterial(IlluminationSet* illuminationSet_)
    : Material(illuminationSet_) {
}
SimpleTextureMaterial::SimpleTextureMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_)
    : Material(illuminationSet_),
      diffuseText(diffuseText_) {
    unsigned int program = illuminationSet_->getShader()->getProgram();

    uColorTex = glGetUniformLocation(program, "colorTex");
}
TextureMaterial::TextureMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_, Texture* emissiveText_, Texture* specularText_)
    : SimpleTextureMaterial(illuminationSet_, diffuseText_),
      emissiveText(emissiveText_),
      specularText(specularText_) {
    unsigned int program = illuminationSet_->getShader()->getProgram();

    uEmiTex = glGetUniformLocation(program, "emiTex");
    uSpecTex = glGetUniformLocation(program, "specularTex");
}
BumpMaterial::BumpMaterial(IlluminationSet* illuminationSet_, Texture* diffuseText_, Texture* emissiveText_, Texture* specularText_, Texture* normalsText_)
    : TextureMaterial(illuminationSet_, diffuseText_, emissiveText_, specularText_),
      normalsText(normalsText_) {
    unsigned int program = illuminationSet_->getShader()->getProgram();

    uNormTex = glGetUniformLocation(program, "normalTex");
}
IlluminationSet* Material::getIlluminationSet() const {
    return illuminationSet;
}

void Material::addNode(Node* node) {
    nodes.push_back(node);
}
void Material::renderNodes(const glm::mat4& view) const {
    activateMaterial(view);
    for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
        (*nodeIt)->renderNode();
    }
}
void SimpleMaterial::activateMaterial(const glm::mat4& view) const {
}
void SimpleTextureMaterial::activateMaterial(const glm::mat4& view) const {
    if (uColorTex != -1 && diffuseText) {
        diffuseText->sendToShaderProgram(uColorTex);
    }
}
void TextureMaterial::activateMaterial(const glm::mat4& view) const {
    if (uColorTex != -1 && diffuseText) {
        diffuseText->sendToShaderProgram(uColorTex);
    }
    if (uEmiTex != -1 && emissiveText) {
        emissiveText->sendToShaderProgram(uEmiTex);
    }
    if (uSpecTex != -1 && specularText) {
        specularText->sendToShaderProgram(uSpecTex);
    }
}
void BumpMaterial::activateMaterial(const glm::mat4& view) const {
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