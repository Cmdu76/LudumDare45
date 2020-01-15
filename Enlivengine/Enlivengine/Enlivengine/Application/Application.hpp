#pragma once

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/Profiler.hpp>

#include <Enlivengine/Graphics/ScreenshotSystem.hpp>

#include <Enlivengine/Application/AudioSystem.hpp>
#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Application/StateManager.hpp>

namespace en
{

class Application
{
	public:
		Application();
		~Application();

		Window& getWindow();
		ResourceManager& getResourceManager();
		AudioSystem& getAudio();
		ScreenshotSystem& getScreenshotSystem();

		template <typename State, typename ... Args>
		void start(Args&& ... args);
		void stop();

		template <typename T, typename ... Args>
		void pushState(Args&& ... args);
		void popState();
		void clearStates();

		U32 getFPS() const;
		U32 getTotalFrames() const;
		Time getTotalDuration() const;

	private:
		void run();
		void events();
		void preUpdate();
		void update(Time dt);
		void postUpdate();
		void render();

	private:
		StateManager mStates;
		Window mWindow;
		ResourceManager mResourceManager;
		AudioSystem mAudioSystem;
		ScreenshotSystem mScreenshotSystem;

		EnSlot(en::Window, onWindowClosed, mWindowClosedSlot);

		U32 mFps;
		bool mRunning;

		U32 mTotalFrames;
		Clock mTotalDuration;

#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiLogger mLogger;
#endif // ENLIVE_ENABLE_IMGUI
};

template <typename State, typename ... Args>
void Application::start(Args&& ... args)
{
	pushState<State>(std::forward<Args>(args)...);

	mStates.applyPendingChanges();

	mRunning = true;

	run();
}

template <typename T, typename ... Args>
void Application::pushState(Args&& ... args)
{
	mStates.pushState<T>(std::forward<Args>(args)...);
}

} // namespace en
