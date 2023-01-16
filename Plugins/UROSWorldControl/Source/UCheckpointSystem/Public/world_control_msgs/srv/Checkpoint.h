#pragma once
#include "ROSBridgeSrv.h"

class UCHECKPOINTSYSTEM_API FCheckpointSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FCheckpointSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
          FString Command;
          FString Args;


	public:
		Request() {}

		Request(FString InCommand, FString InArgs)
		{
			Command = InCommand;
			Args = InArgs;
		}

		FString GetCommand()
		{
			return Command;
		}

		FString GetArgs() {
			return Args;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Command = JsonObject->GetStringField("command");
			Args = JsonObject->GetStringField("args");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
                        return "FROSSetMaterialSrv:Request {command = " + Command + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("command"), Command);
			Object->SetStringField(TEXT("args"), Args);
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		bool Success;

	public:
		Response() {}

		Response(bool InSuccess)
		{
			Success = InSuccess;
		}

		bool GetSuccess()
		{
			return Success;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Success = JsonObject->GetBoolField("success");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return "FROSSetMaterialSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};
