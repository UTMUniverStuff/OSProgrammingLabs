os_img=$1
vm_name="SOMIPP"

VBoxManage startvm $vm_name
VBoxManage controlvm $vm_name reset;
