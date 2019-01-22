

using UnrealBuildTool;
using System.Collections.Generic;

public class UntoldTarget : TargetRules
{
	public UntoldTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Untold" } );
	}
}
