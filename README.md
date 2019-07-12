# python3-aa
A python3 C extension for interfacing with AppArmor 

## Build Dependencies
```bash
sudo apt install build-essential libapparmor-dev python3-dev
```

## Building
```bash
python3 setup.py build
```

## Installing
```bash
sudo python3 setup.py install --record files.txt
```

## Uninstalling
```bash
sudo xargs rm -rf < files.txt
```
