#include "stdafx.h"
#include "LayerWindow.h"
#include "Editor.h"

using namespace wiECS;
using namespace wiScene;


LayerWindow::LayerWindow(EditorComponent* editor) : GUI(&editor->GetGUI())
{
	assert(GUI && "Invalid GUI!");

	window = new wiWindow(GUI, "Layer Window");
	window->SetSize(XMFLOAT2(410, 160));
	GUI->AddWidget(window);

	float x = 30;
	float y = 0;
	float step = 25;
	float siz = 20;

	for (uint32_t i = 0; i < arraysize(layers); ++i)
	{
		layers[i] = new wiCheckBox("");
		layers[i]->SetText(std::to_string(i) + ": ");
		layers[i]->SetPos(XMFLOAT2(x + (i % 8) * 50, y + (i / 8 + 1) * step));
		layers[i]->OnClick([=](wiEventArgs args) {

			LayerComponent* layer = wiScene::GetScene().layers.GetComponent(entity);
			if (layer == nullptr)
			{
				layer = &wiScene::GetScene().layers.Create(entity);
			}

			if (args.bValue)
			{
				layer->layerMask |= 1 << i;
			}
			else
			{
				layer->layerMask &= ~(1 << i);
			}

			});
		window->AddWidget(layers[i]);
	}

	y += step * 5;

	enableAllButton = new wiButton("Enable ALL");
	enableAllButton->SetPos(XMFLOAT2(x, y));
	enableAllButton->OnClick([this](wiEventArgs args) {
		LayerComponent* layer = wiScene::GetScene().layers.GetComponent(entity);
		if (layer == nullptr)
			return;
		layer->layerMask = ~0;
	});
	window->AddWidget(enableAllButton);

	enableNoneButton = new wiButton("Enable NONE");
	enableNoneButton->SetPos(XMFLOAT2(x + 120, y));
	enableNoneButton->OnClick([this](wiEventArgs args) {
		LayerComponent* layer = wiScene::GetScene().layers.GetComponent(entity);
		if (layer == nullptr)
			return;
		layer->layerMask = 0;
	});
	window->AddWidget(enableNoneButton);

	window->Translate(XMFLOAT3((float)wiRenderer::GetDevice()->GetScreenWidth() - 450, 300, 0));
	window->SetVisible(false);

	SetEntity(INVALID_ENTITY);
}

LayerWindow::~LayerWindow()
{
	window->RemoveWidgets(true);
	GUI->RemoveWidget(window);
	delete window;
}

void LayerWindow::SetEntity(Entity entity)
{
	this->entity = entity;

	if (entity != INVALID_ENTITY)
	{
		window->SetEnabled(true);

		LayerComponent* layer = wiScene::GetScene().layers.GetComponent(entity);
		if (layer == nullptr)
		{
			for (uint32_t i = 0; i < 32; ++i)
			{
				layers[i]->SetCheck(true);
			}
		}
		else
		{
			for (uint32_t i = 0; i < 32; ++i)
			{
				layers[i]->SetCheck(layer->GetLayerMask() & 1 << i);
			}
		}
	}
	else
	{
		window->SetEnabled(false);
	}
}
