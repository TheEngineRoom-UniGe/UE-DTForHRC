// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#if ENGINE_MINOR_VERSION > 23 || ENGINE_MAJOR_VERSION >4
#include "Modules/ModuleManager.h"
#else
#include "ModuleManager.h"
#endif

class FUCheckpointSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

// DECLARE_LOG_CATEGORY_EXTERN(LogCheckpoint, Log, All);
