#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
void Reset(void);
void Login(void);
void AcssesAllowed(void);
void WrongPass(void);
void NewTry(void);
void AccessDenied(void);
void LightSensorCheck(void);
void CloseDoor(void);
void OpenDoor(void);
void GreenLed(u8 check);
void RedLed(u8 check);
void openFan(f32 Duty);
void closeFan(void);
void ShowTemp(void);
void checkTemp(void);
void DashBoard (void);
void LightControl(void);
void Alert(u8 check);
#endif
