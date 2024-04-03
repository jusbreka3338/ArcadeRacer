// Fill out your copyright notice in the Description page of Project Settings.


#include "StringHelper.h"

StringHelper::StringHelper()
{
}

StringHelper::~StringHelper()
{
}

void StringHelper::Print(FString inString)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, inString);
}

FString StringHelper::IntToString(int inInt)
{
	return FString::Printf(TEXT("%i"), inInt);
}
