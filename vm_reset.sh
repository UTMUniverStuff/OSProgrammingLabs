os_img=$1
vm_name="SOMIPP"

VBoxManage startvm $vm_name 2>/dev/null
VBoxManage controlvm $vm_name reset;
