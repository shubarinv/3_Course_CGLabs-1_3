//
// Created by Vladimir Shubarin on 30.11.2020.
//

#ifndef CGLABS__MATERIAL_HPP_
#define CGLABS__MATERIAL_HPP_

class Material {
  glm::vec3 ambient{};
  glm::vec3 diffuse{};
  glm::vec3 specular{};
  float shininess{0};
  Material(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess) {
  ambient = _ambient;
  diffuse = _diffuse;
  specular = _specular;
  shininess = _shininess;
  }
};

#endif //CGLABS__MATERIAL_HPP_
