#pragma once

#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHandleShake, Log, All);

class PaladinHandleShakeTrackEditorModule :public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FDelegateHandle HandleShakeTrackEditorHandle;
};