# MarkovLibrary

各种算法的实验室
------------------------------------------
|              UnrealPlugin               |
------------------------------------------
| +EnvironmentInterface: IEnvironment     |
| +Communication: CommunicationHandler    |
------------------------------------------
| +CreateEnvironment()                    |
| +RegisterEnvironment()                  |
| +WaitForInstructions()                  |
| +SendObservations()                     |
------------------------------------------

------------------------------------------
|           IEnvironment                  |
------------------------------------------
| +GetObservation()                       |
| +TakeAction()                           |
| +Reset()                                |
| +IsDone()                               |
------------------------------------------

------------------------------------------
|     CommunicationHandler                |
------------------------------------------
| +PythonConnection: PythonConnection     |
------------------------------------------
| +EstablishConnection()                  |
| +ReceiveCommand()                       |
| +SendData()                             |
------------------------------------------

------------------------------------------
|        PythonConnection                 |
------------------------------------------
| +Socket: socket                         |
| +Address: sockaddr_in                   |
------------------------------------------
| +Connect()                              |
| +Receive()                              |
| +Send()                                 |
------------------------------------------

