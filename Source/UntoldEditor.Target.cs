

using UnrealBuildTool;
using System.Collections.Generic;

public class UntoldEditorTarget : TargetRules
{
	public UntoldEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Untold" } );
	}
}
