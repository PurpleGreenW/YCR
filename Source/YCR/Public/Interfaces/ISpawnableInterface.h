// ISpawnableInterface.h
UINTERFACE(MinimalAPI, Blueprintable)
class USpawnableInterface : public UInterface
{
	GENERATED_BODY()
};

class YCR_API ISpawnableInterface
{
	GENERATED_BODY()

public:
	// Initialisierung nach dem Spawn
	virtual void OnSpawned() = 0;
    
	// Rückgabe zum Pool statt Destroy
	virtual void OnDespawned() = 0;
    
	// Position/Rotation Setup
	virtual void SetSpawnTransform(const FTransform& Transform) = 0;
    
	// Spawn-spezifische Daten
	virtual void SetSpawnData(const FSpawnData& Data) = 0;
    
	// Für Pool-System
	virtual bool IsReadyToSpawn() const = 0;
	virtual void ResetSpawnableState() = 0;
};