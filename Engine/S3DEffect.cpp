#include "TExtureEffect.h"
#include "S3Dpipeline.h"


void Software3D::TextureEffect::BindTexture(const std::wstring& filename) {
	
	texture = std::make_unique<Surface>(Surface::FromFile(filename));
	textureWidth = float(texture->GetWidth());
	textureHeight = float(texture->GetHeight());
	
	uVals = new int[texture->GetWidth()];
	vVals = new int[texture->GetHeight()];

	pipeline->BindTexture(filename);
}

void Software3D::TextureEffect::RenderObjectWithTexture(const IndexedList<TextureVertex>& object) {
	
	pipeline->DrawSimple(object, this);
}