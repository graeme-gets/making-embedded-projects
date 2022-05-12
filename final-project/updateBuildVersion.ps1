$file = "..\Core\Cli\Inc\version.h"
$version = Get-Content $file | Select-String 'VERSION_STRING' 
$regex = [Regex]::new('(?<=").*(?=")')
$match = $regex.Match($version)

$oldVersion = [version]::new($match) 
$newVersion = [version]::New($oldVersion.Major,$oldVersion.Minor,$oldVersion.Build,$oldVersion.Revision+1)
Write-Host "version -> $($version)"
Write-Host "version -> $($match)"
Write-Host "version -> $($newVersion)"

# Now replace the string in the file

(Get-Content $file).Replace($oldVersion, $newVersion) | Set-Content $file