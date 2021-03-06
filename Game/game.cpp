#include "game.h"
#include "GameData.h"
#include "drawdata.h"
#include "DrawData2D.h"
#include "gameobject.h"
#include "ObjectList.h"
#include "helper.h"
#include <windows.h>
#include <time.h>
#include "DDSTextureLoader.h"
#include <d3d11shader.h>

using namespace DirectX;

Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance) :m_playTime(0), m_fxFactory(nullptr), m_states(nullptr)
{
	//Create DirectXTK spritebatch stuff
	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"italic.spritefont"));

	//seed the random number generator
	srand((UINT)time(NULL));

	//Direct Input Stuff
	m_hWnd = _hWnd;
	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pMouse->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//create GameData struct and populate its pointers
	m_GD = new GameData;
	GameDataSetup(m_GD);
	//set up DirectXTK Effects system
	m_fxFactory = new EffectFactory(_pd3dDevice);

	//Tell the fxFactory to look to the correct build directory to pull stuff in from
#ifdef DEBUG
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Debug");
#else
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Release");
#endif

	// Create other render resources here
	m_states = new CommonStates(_pd3dDevice);

	//init render system for VBGOs
	VBGO::Init(_pd3dDevice);

	//find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	//Add window size into Gamedata
	m_GD->m_scrWidth = (width - 200) / 50;
	m_GD->m_scrHeight = (height - 200)/ 50;

	//create a base camera
	m_cam = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);
	m_cam->SetPos(Vector3(0.0f, 100.0f, 0.01f));
	m_GameObjects.push_back(m_cam);

	//create a base light
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_GameObjects.push_back(m_light);

	//create DrawData struct and populate its pointers
	m_DD = new DrawData;
	m_DD->m_pd3dImmediateContext = nullptr;
	m_DD->m_states = m_states;
	m_DD->m_cam = m_cam;
	m_DD->m_light = m_light;

	//add a player
	Player* pPlayer = new Player("BirdModelV1.cmo", _pd3dDevice, m_fxFactory);
	m_GameObjects.push_back(pPlayer);




	//add a secondary camera
	m_TPScam = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 10.0f, 50.0f));
	m_GameObjects.push_back(m_TPScam);

	/*Terrain* terrain = new Terrain("table.cmo", _pd3dDevice, m_fxFactory, Vector3(100.0f, 0.0f, 100.0f), 0.0f, 0.0f, 0.0f, 0.25f * Vector3::One);
	m_GameObjects.push_back(terrain);

	//add some stuff to show off
	*/
	/*FileVBGO* terrainBox = new FileVBGO("../Assets/terrainTex.txt", _pd3dDevice);
	m_GameObjects.push_back(terrainBox);*/

/*	FileVBGO* Box = new FileVBGO("../Assets/cube.txt", _pd3dDevice);
	m_GameObjects.push_back(Box);
	Box->SetPos(Vector3(0.0f, 0.0f, -100.0f));
	Box->SetPitch(XM_PIDIV4);
	Box->SetScale(20.0f);*/
	
	//L-system like tree
	//m_GameObjects.push_back(new Tree(4, 4, .6f, 10.0f *Vector3::Up, XM_PI/6.0f, "JEMINA vase -up.cmo", _pd3dDevice, m_fxFactory));

	VBCube* cube = new VBCube();
	cube->init(20, _pd3dDevice);
	cube->SetPos(Vector3(0.0f, 0.0f, 0.0f));
	cube->SetScale(1.0f);
	m_GameObjects.push_back(cube);

	/*VBSpike* spikes = new VBSpike();
	spikes->init(11, _pd3dDevice);
	spikes->SetPos(Vector3(0.0f, 0.0f, 100.0f));
	spikes->SetScale(4.0f);
	m_GameObjects.push_back(spikes);

	VBSpiral* spiral = new VBSpiral();
	spiral->init(11, _pd3dDevice);
	spiral->SetPos(Vector3(-100.0f, 0.0f, 0.0f));
	spiral->SetScale(4.0f);
	m_GameObjects.push_back(spiral);

	VBPillow* pillow = new VBPillow();
	pillow->init(11, _pd3dDevice);
	pillow->SetPos(Vector3(-100.0f, 0.0f, -100.0f));
	pillow->SetScale(4.0f);
	m_GameObjects.push_back(pillow);

	VBSnail* snail = new VBSnail(_pd3dDevice, "../Assets/baseline.txt", 150, 0.98, 0.09f * XM_PI, 0.4f, Color(1.0f, 0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f, 1.0f));
	snail->SetPos(Vector3(-100.0f, 0.0f, 100.0f));
	snail->SetScale(2.0f);
	m_GameObjects.push_back(snail);

	ImageGO2D* logo = new ImageGO2D("logo_small", _pd3dDevice);
	logo->SetPos(200.0f * Vector2::One);
	m_GameObject2Ds.push_back(logo); */

	/*TextGO2D* text = new TextGO2D("");
	text->SetPos(Vector2(100, 10));
	text->SetColour(Color((float*)&Colors::Yellow));
	text->SetScale(0.5f);
	//text->SetText(text->TextSetup(m_GD));
	m_GameObject2Ds.push_back(text);*/

	/*TextGO2D* m_Scrint = new TextGO2D("Width: " + std::to_string(m_GD->m_scrWidth) + " Height: " + std::to_string(m_GD->m_scrHeight));
	m_Scrint->SetPos(Vector2(90, 10));
	m_Scrint->SetColour(Color((float*)&Colors::Yellow));
	m_Scrint->SetScale(0.5f);
	m_GameObject2Ds.push_back(m_Scrint);*/

	//creation of the base grid
	m_Grid = new Grid(_pd3dDevice, m_GD);
	m_GameObjects.push_back(m_Grid);

	m_GD->m_TGS = GS_BASICTEXT;
	m_GD->m_GS = GS_NULL;
}

Game::~Game()
{
	//delete Game Data & Draw Data
	delete m_GD;
	delete m_DD;

	//tidy up VBGO render system
	VBGO::CleanUp();

	//tidy away Direct Input Stuff
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
	}

	//get rid of the game objects here
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		delete (*it);
	}
	m_GameObjects.clear();

	//and the 2D ones
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		delete (*it);
	}
	m_GameObject2Ds.clear();

	//clear away CMO render system
	delete m_states;
	delete m_fxFactory;

	delete m_DD2D;
}

bool Game::Update()
{
	//Poll Keyboard & Mouse
	ReadInput();

	//Upon pressing escape QUIT
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return false;
	}

	//lock the cursor to the centre of the window
	RECT window;
	GetWindowRect(m_hWnd, &window);
	// SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);

	GetGameState();

	/*switch (*m_keyboardState)
	{
		case (DIK_F1 & 0x80) && (! DIK_F1 & 0x80):
			if (m_GD->m_TGS == GS_BASICTEXT)
			{
				m_GD->m_TGS = GS_HELPTEXT;
			}
			else
			{
				m_GD->m_TGS = GS_BASICTEXT;
			}
			break;
	}*/

	if ((m_keyboardState[DIK_F1] & 0x80) && !(m_prevKeyboardState[DIK_F1] & 0x80))
	{
		if (m_GD->m_TGS == GS_BASICTEXT)
		{
			m_GD->m_TGS = GS_HELPTEXT;
		}
		else
		{
			m_GD->m_TGS = GS_BASICTEXT;
		}
	}

	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	m_GD->m_dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;

	//update all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(m_GD);
	}
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->Tick(m_GD);
	}

	return true;
}

void Game::GetGameState()
{
	if ((m_keyboardState[DIK_F1] & 0x80) && !(m_prevKeyboardState[DIK_F1] & 0x80))
	{
		if (m_GD->m_TGS == GS_BASICTEXT)
		{
			m_GD->m_TGS = GS_HELPTEXT;
		}
		else
		{
			m_GD->m_TGS = GS_BASICTEXT;
		}
	}
}

void Game::Render(ID3D11DeviceContext* _pd3dImmediateContext)
{
	//set immediate context of the graphics device
	m_DD->m_pd3dImmediateContext = _pd3dImmediateContext;

	//set which camera to be used
	m_DD->m_cam = m_cam;
	/*if (m_GD->m_GS == GS_PLAY_TPS_CAM)
	{
		m_DD->m_cam = m_TPScam;
	}*/

	//update the constant buffer for the rendering of VBGOs
	VBGO::UpdateConstantBuffer(m_DD);

	//draw all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Draw(m_DD);
	}

	// Draw sprite batch stuff
	m_DD2D->m_Sprites->Begin();
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->Draw(m_DD2D);
	}
	m_DD2D->m_Sprites->End();

	//drawing text screws up the Depth Stencil State, this puts it back again!
	_pd3dImmediateContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
}

bool Game::ReadInput()
{
	//copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(unsigned char) * 256);

	//clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(unsigned char) * 256);
	ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));

	// Read the keyboard device.
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	// Read the Mouse device.
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the Mouse lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Game::GameDataSetup(GameData* _GD)
{
	_GD->m_keyboardState = m_keyboardState;
	_GD->m_prevKeyboardState = m_prevKeyboardState;
	_GD->m_GS = GS_NULL;
	_GD->m_mouseState = &m_mouseState;
	_GD->m_t = 0.5f;
	_GD->m_TextToggle = false;
	_GD->m_HelpText = false;
	_GD->m_F1 = true;
	_GD->m_Pause = false;
	_GD->m_DebugPrint = 0;
}