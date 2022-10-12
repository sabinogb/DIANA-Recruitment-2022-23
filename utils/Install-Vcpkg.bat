if exist "vcpkg-windows" (
 echo "Install Vcpkg : vcpkg-windows folder already existing. To install vcpkg using this script, delete ./vcpkg-windows folder."
) ELSE (
if exist "vcpkg" (
echo "Install Vcpkg : vcpkg-windows folder already existing. To install vcpkg using this script, delete ./vcpkg folder."
)
echo "Installing VCPKG..."
 git clone https://github.com/Microsoft/vcpkg.git
 ren vcpkg vcpkg-windows
 ./vcpkg-windows/bootstrap-vcpkg.bat
 )
