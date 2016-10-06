$exe = ".\Sample_03_Matchmaking_Client_cpp.exe"

For($i=0; $i -lt 3; $i++)
{
    Start-Process -FilePath $exe  -ArgumentList "ux$i", "TeamDeathMatch"
}