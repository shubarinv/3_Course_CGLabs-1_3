//
// Created by Vladimir Shubarin on 04.01.2021.
//

#ifndef CGLABS__TEXTURE_HPP_
#define CGLABS__TEXTURE_HPP_
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Resource.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include "functions.hpp"
class Texture {
  GL::Texture2D _texture;
  Texture(std::string filepath) {
	PluginManager::Manager<Trade::AbstractImporter> manager;
	Containers::Pointer<Trade::AbstractImporter> importer =manager.loadAndInstantiate("AnyImageImporter");
	if (!importer) std::exit(1);

	if (!importer->openFile("/path/to/myfile.jpg"))
	  std::exit(2);
	/* Set texture data and parameters */
	Containers::Optional<Trade::ImageData2D> image = importer->image2D(0);
	CORRADE_INTERNAL_ASSERT(image);
	_texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
		.setMagnificationFilter(GL::SamplerFilter::Linear)
		.setMinificationFilter(GL::SamplerFilter::Linear)
		.setStorage(1, GL::textureFormat(image->format()), image->size())
		.setSubImage(0, {}, *image);
	Containers::Optional<Trade::ImageData2D> image = importer->image2D(0);
	CORRADE_INTERNAL_ASSERT(image);
  }
};

#endif//CGLABS__TEXTURE_HPP_
