#include "Material.h"

unsigned int Material::numMaterials = 0;

std::shared_ptr<SimpleMaterial> SimpleMaterial::createSimpleMaterial(std::shared_ptr<IlluminationSet> illuminationSet_) {
    std::shared_ptr<SimpleMaterial> material(new SimpleMaterial(illuminationSet_));
    illuminationSet_->addMaterial(material);
    return material;
}
std::shared_ptr<SimpleTextureMaterial> SimpleTextureMaterial::createSimpleTextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_) {
    std::shared_ptr<SimpleTextureMaterial> material(new SimpleTextureMaterial(illuminationSet_, diffuseText_));
    illuminationSet_->addMaterial(material);
    return material;
}
std::shared_ptr<TextureMaterial> TextureMaterial::createTextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_) {
    std::shared_ptr<TextureMaterial> material(new TextureMaterial(illuminationSet_, diffuseText_, emissiveText_, specularText_));
    illuminationSet_->addMaterial(material);
    return material;
}
std::shared_ptr<BumpMaterial> BumpMaterial::createBumpMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_, std::shared_ptr<Texture> normalsText_) {
    std::shared_ptr<BumpMaterial> material(new BumpMaterial(illuminationSet_, diffuseText_, emissiveText_, specularText_, normalsText_));
    illuminationSet_->addMaterial(material);
    return material;
}

Material::Material(std::shared_ptr<IlluminationSet> illuminationSet_)
    : illuminationSet(illuminationSet_),
      id(numMaterials++) {
    std::cout << "Created material: " << id << std::endl;
}
SimpleMaterial::SimpleMaterial(std::shared_ptr<IlluminationSet> illuminationSet_)
    : Material(illuminationSet_) {
}
SimpleTextureMaterial::SimpleTextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_)
    : Material(illuminationSet_),
      diffuseText(diffuseText_) {
    unsigned int program = illuminationSet_->getShader()->getProgram();

    uColorTex = glGetUniformLocation(program, "colorTex");
}
TextureMaterial::TextureMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_)
    : SimpleTextureMaterial(illuminationSet_, diffuseText_),
      emissiveText(emissiveText_),
      specularText(specularText_) {
    unsigned int program = illuminationSet_->getShader()->getProgram();

    uEmiTex = glGetUniformLocation(program, "emiTex");
    uSpecTex = glGetUniformLocation(program, "specularTex");
}
BumpMaterial::BumpMaterial(std::shared_ptr<IlluminationSet> illuminationSet_, std::shared_ptr<Texture> diffuseText_, std::shared_ptr<Texture> emissiveText_, std::shared_ptr<Texture> specularText_, std::shared_ptr<Texture> normalsText_)
    : TextureMaterial(illuminationSet_, diffuseText_, emissiveText_, specularText_),
      normalsText(normalsText_) {
    unsigned int program = illuminationSet_->getShader()->getProgram();

    uNormTex = glGetUniformLocation(program, "normalTex");
}
Material::~Material() {
    std::cout << "Deleted material: " << id << std::endl;
}
std::shared_ptr<IlluminationSet> Material::getIlluminationSet() const {
    return illuminationSet;
}

void Material::addNode(std::shared_ptr<Node> node) {
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