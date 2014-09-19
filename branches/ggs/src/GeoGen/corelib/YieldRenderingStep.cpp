#include "../InternalErrorException.hpp"
#include "YieldRenderingStep.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../renderer/Renderer.hpp"
#include "../Rectangle.hpp"
#include "../renderer/RendererObject.hpp"
#include "../genlib/HeightMap.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace renderer;
using namespace genlib;

void YieldRenderingStep::Step(renderer::Renderer* renderer) const
{
	HeightMap* internalData = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightMap* copiedData = new HeightMap(*internalData, dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle());

	if (!renderer->GetRenderedMapTable().AddItem(this->name, copiedData))
	{
		delete copiedData;
	}
}

void YieldRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->CombineRectangle(this->rect);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		this->GetRenderingBounds(renderer));
}

void YieldRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->rect.Serialize(stream);

	stream << GG_STR(", ") << this->name;
}