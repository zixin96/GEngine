#include "GLCore.h"
#include "BatchRenderingLayer.h"

using namespace GLCore;

class BatchRenderingApp : public Application
{
public:
	BatchRenderingApp()
		: Application("Batch Rendering")
	{
		PushLayer(new BatchRenderingLayer());
	}
};

int main()
{
	std::unique_ptr<BatchRenderingApp> app = std::make_unique<BatchRenderingApp>();
	app->Run();
}