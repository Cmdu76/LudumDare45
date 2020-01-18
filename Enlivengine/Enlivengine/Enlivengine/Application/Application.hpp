#pragma once

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/Profiler.hpp>

#include <Enlivengine/Graphics/ScreenshotSystem.hpp>
#include <Enlivengine/Application/PathManager.hpp>
#include <Enlivengine/Application/AudioSystem.hpp>
#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Application/StateManager.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>

namespace en
{

class Application
{
	ENLIVE_SINGLETON(Application);

public:
	Window& GetWindow();
	ScreenshotSystem& GetScreenshotSystem();

	template <typename State, typename ... Args>
	void Start(Args&& ... args);
	void Stop();

	template <typename T, typename ... Args>
	void PushState(Args&& ... args);
	void PopState();
	void ClearStates();

	U32 GetFPS() const;
	U32 GetTotalFrames() const;
	Time GetTotalDuration() const;

private:
	~Application();

	void Run();
	void Events();
	void PreUpdate();
	void Update(Time dt);
	void PostUpdate();
	void Render();

private:
	StateManager mStates;
	Window mWindow;
	ScreenshotSystem mScreenshotSystem;

	EnSlot(en::Window, onWindowClosed, mWindowClosedSlot);

	U32 mFps;
	bool mRunning;

	U32 mTotalFrames;
	Clock mTotalDuration;
};

template <typename State, typename ... Args>
void Application::Start(Args&& ... args)
{
	PushState<State>(std::forward<Args>(args)...);

	mStates.applyPendingChanges();

	mRunning = true;

	Run();
}

template <typename T, typename ... Args>
void Application::PushState(Args&& ... args)
{
	mStates.pushState<T>(std::forward<Args>(args)...);
}

} // namespace en
