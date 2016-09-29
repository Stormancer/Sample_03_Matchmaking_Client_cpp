$exe = ".\Sample_03_Matchmaking_Client_cpp.exe"

For($i=0; $i -lt 4; $i++)
{
    Start-Process -FilePath $exe  -ArgumentList "u$i", "TeamDeathMatch"
}