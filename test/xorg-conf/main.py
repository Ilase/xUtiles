import os
import subprocess
import argparse

def create_xorg_config(vendor_name, model_name, driver, depth, gamma, hz):
    config_content = f"""
Section "ServerLayout"
    Identifier     "X.org Configured"
    Screen      0  "Screen0" 0 0
    InputDevice    "Mouse0" "CorePointer"
    InputDevice    "Keyboard0" "CoreKeyboard"
EndSection

Section "Files"
    ModulePath   "/usr/lib/xorg/modules"
    FontPath     "/usr/share/fonts/X11/misc"
    FontPath     "/usr/share/fonts/X11/cyrillic"
    FontPath     "/usr/share/fonts/X11/100dpi/:unscaled"
    FontPath     "/usr/share/fonts/X11/75dpi/:unscaled"
    FontPath     "/usr/share/fonts/X11/Type1"
    FontPath     "/usr/share/fonts/X11/100dpi"
    FontPath     "/usr/share/fonts/X11/75dpi"
    FontPath     "built-ins"
EndSection

Section "Module"
    Load  "glx"
EndSection

Section "InputDevice"
    Identifier  "Keyboard0"
    Driver      "kbd"
EndSection

Section "InputDevice"
    Identifier  "Mouse0"
    Driver      "mouse"
    Option      "Protocol" "auto"
    Option      "Device" "/dev/input/mice"
    Option      "ZAxisMapping" "4 5 6 7"
EndSection

Section "Monitor"
    Identifier   "Monitor0"
    VendorName   "{vendor_name}"
    ModelName    "{model_name}"
    Gamma "{gamma}"
    VertRefresh "{hz}"
EndSection

Section "Device"
    Identifier  "Device0"
    Driver      "{driver}"
    VendorName "NVIDIA Corporation"
EndSection

Section "Screen"
    Identifier "Screen0"
    Device     "Device0"
    Monitor    "Monitor0"
    SubSection "Display"
        Viewport   0 0
        Depth     {depth}
    EndSubSection
EndSection
"""
    # Путь для сохранения конфигурационного файла Xorg
    config_file_path = "/etc/X11/xorg1.conf"
    
    try:
        with open(config_file_path, 'w') as config_file:
            config_file.write(config_content.strip())
        print(f"Конфигурационный файл Xorg успешно создан по пути: {config_file_path}")

        # Запросить перезапуск Xorg
       
        subprocess.run(["systemctl", "restart", "display-manager"], check=True)
        print("Xorg перезапущен.")

    except Exception:
        print(f"")

if __name__ == "__main__":
    # Настройка аргументов командной строки
    parser = argparse.ArgumentParser(description="Создание конфигурационного файла Xorg.")
    parser.add_argument("-vendor_name", required=True, help="Имя производителя монитора")
    parser.add_argument("-model_name", required=True, help="Модель монитора")
    parser.add_argument("-driver", required=True, help="Драйвер (например, vmware, nvidia, intel и т. д.)")
    parser.add_argument("-hz", required=True, help="VertRefresh Герцовка монитора")
    parser.add_argument("-gamma", required=True, help="Gamma от 0.1 до 10.0, по умолчанию 1.0")
    parser.add_argument("-depth", required=True, help="""Depth 1: Черно-белое изображение (1 бит на пиксель).
Depth 4: Палитра с 16 цветами (4 бита на пиксель).
Depth 8: Палитра с 256 цветами (8 бит на пиксель).
Depth 15: 32768 цветов (15 бит на пиксель).
Depth 16: 65536 цветов (16 бит на пиксель).
Depth 24: Более 16 миллионов цветов (24 бита на пиксель).""")

    args = parser.parse_args()
    
    create_xorg_config(args.vendor_name, args.model_name, args.driver, args.depth, args.gamma, args.hz)
