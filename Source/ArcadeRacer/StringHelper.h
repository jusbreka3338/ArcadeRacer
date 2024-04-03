// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ARCADERACER_API StringHelper
{
public:
	StringHelper();
	~StringHelper();

	static void Print(FString inString);
	static FString IntToString(int inInt);
};
