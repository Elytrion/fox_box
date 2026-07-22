#pragma once

class App
{
public:
	App();
	~App();

	void run();
	bool isRunning() const;
private:
	void initImGUI();
	void shutdownImGUI();
	void beginFrameImGUI();
	void endFrameImGUI();
};