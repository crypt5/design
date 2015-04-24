#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "graphics.h"
#include "logger.h"

struct module_t{
  WIDGET* op_label;
  WIDGET* op_force;
  WIDGET* op_disp;
  WIDGET* desired;
  WIDGET* desired_lab;
  WIDGET* desired_in;
  WIDGET* desired_unit;
  WIDGET* start;
  WIDGET* stop;
  WIDGET* current;
  WIDGET* current_lab;
  WIDGET* current_out;
  WIDGET* current_unit;
};

struct ex_force_t{
  WIDGET* label;
  WIDGET* output;
  WIDGET* unit;
};

struct ex_grip_t{
  WIDGET* disp_lab;
  WIDGET* disp_out;
  WIDGET* disp_unit;
  WIDGET* force_lab;
  WIDGET* force_out;
  WIDGET* force_unit;
};
static WIDGET* file;
static struct module_t mod_one;
static struct module_t mod_two;
static struct ex_force_t ex_force;
static struct ex_grip_t ex_grip;

void button_group_one(GUI* g,WIDGET* w,void* data)
{
  if(strcmp(get_radio_button_text(w),"Force Mode")==0){
    int check=get_radio_button_check(w);
    if(check==0){
      set_radio_button_check(data,1);
      set_label_text(mod_one.current_lab,"Displacement:");
      set_label_text(mod_one.desired,"Displacement:");
      set_label_text(mod_one.desired_unit,"inches");
      set_label_text(mod_one.current_unit,"inches");
    }
    else{
      set_radio_button_check(data,0);
      set_label_text(mod_one.current_lab,"Force:");
      set_label_text(mod_one.desired,"Force:");
      set_label_text(mod_one.desired_unit,"lbs");
      set_label_text(mod_one.current_unit,"lbs");
    }

  }
  else{
    int check=get_radio_button_check(w);
    if(check==1){
      set_radio_button_check(data,0);
      set_label_text(mod_one.current_lab,"Displacement:");
      set_label_text(mod_one.desired,"Displacement:");
      set_label_text(mod_one.desired_unit,"inches");
      set_label_text(mod_one.current_unit,"inches");
    }
    else{
      set_radio_button_check(data,1);
      set_label_text(mod_one.current_lab,"Force:");
      set_label_text(mod_one.desired,"Force:");
      set_label_text(mod_one.desired_unit,"lbs");
      set_label_text(mod_one.current_unit,"lbs");
    }

  }

  update_widget(g,mod_one.desired_unit);
  update_widget(g,mod_one.current_unit);
  update_widget(g,mod_one.desired);
  update_widget(g,mod_one.current_lab);
  update_widget(g,w);
  update_widget(g,data);
}

void button_group_two(GUI* g,WIDGET* w,void* data)
{
  if(strcmp(get_radio_button_text(w),"Force Mode")==0){
    int check=get_radio_button_check(w);
    if(check==0){
      set_radio_button_check(data,1);
      set_label_text(mod_two.current_lab,"Displacement:");
      set_label_text(mod_two.desired,"Displacement:");
      set_label_text(mod_two.desired_unit,"inches");
      set_label_text(mod_two.current_unit,"inches");
    }
    else{
      set_radio_button_check(data,0);
      set_label_text(mod_two.current_lab,"Force:");
      set_label_text(mod_two.desired,"Force:");
      set_label_text(mod_two.desired_unit,"lbs");
      set_label_text(mod_two.current_unit,"lbs");
    }

  }
  else{
    int check=get_radio_button_check(w);
    if(check==1){
      set_radio_button_check(data,0);
      set_label_text(mod_two.current_lab,"Displacement:");
      set_label_text(mod_two.desired,"Displacement:");
      set_label_text(mod_two.desired_unit,"inches");
      set_label_text(mod_two.current_unit,"inches");
    }
    else{
      set_radio_button_check(data,1);
      set_label_text(mod_two.current_lab,"Force:");
      set_label_text(mod_two.desired,"Force:");
      set_label_text(mod_two.desired_unit,"lbs");
      set_label_text(mod_two.current_unit,"lbs");
    }

  }

  update_widget(g,mod_two.desired_unit);
  update_widget(g,mod_two.current_unit);
  update_widget(g,mod_two.desired);
  update_widget(g,mod_two.current_lab);
  update_widget(g,w);
  update_widget(g,data);
}

void mod_one_en(GUI* g, WIDGET* w,void* data)
{
  int en=get_checkbox_checked(w);
  set_label_enable(mod_one.op_label,en);
  set_radio_button_enable(mod_one.op_force,en);
  set_radio_button_enable(mod_one.op_disp,en);
  set_label_enable(mod_one.desired,en);
  set_label_enable(mod_one.desired_lab,en);
  set_textfield_enable(mod_one.desired_in,en);
  set_label_enable(mod_one.desired_unit,en);
  set_button_enable(mod_one.start,en);
  set_button_enable(mod_one.stop,en);
  set_label_enable(mod_one.current,en);
  set_label_enable(mod_one.current_lab,en);
  set_textfield_enable(mod_one.current_out,en);
  set_label_enable(mod_one.current_unit,en);

  update_widget(g,mod_one.op_label);
  update_widget(g,mod_one.op_force);
  update_widget(g,mod_one.op_disp);
  update_widget(g,mod_one.desired);
  update_widget(g,mod_one.desired_lab);
  update_widget(g,mod_one.desired_in);
  update_widget(g,mod_one.desired_unit);
  update_widget(g,mod_one.start);
  update_widget(g,mod_one.stop);
  update_widget(g,mod_one.current);
  update_widget(g,mod_one.current_lab);
  update_widget(g,mod_one.current_out);
  update_widget(g,mod_one.current_unit);
}

void mod_two_en(GUI* g, WIDGET* w,void* data)
{
  int en=get_checkbox_checked(w);
  set_label_enable(mod_two.op_label,en);
  set_radio_button_enable(mod_two.op_force,en);
  set_radio_button_enable(mod_two.op_disp,en);
  set_label_enable(mod_two.desired,en);
  set_label_enable(mod_two.desired_lab,en);
  set_textfield_enable(mod_two.desired_in,en);
  set_label_enable(mod_two.desired_unit,en);
  set_button_enable(mod_two.start,en);
  set_button_enable(mod_two.stop,en);
  set_label_enable(mod_two.current,en);
  set_label_enable(mod_two.current_lab,en);
  set_textfield_enable(mod_two.current_out,en);
  set_label_enable(mod_two.current_unit,en);

  update_widget(g,mod_two.op_label);
  update_widget(g,mod_two.op_force);
  update_widget(g,mod_two.op_disp);
  update_widget(g,mod_two.desired);
  update_widget(g,mod_two.desired_lab);
  update_widget(g,mod_two.desired_in);
  update_widget(g,mod_two.desired_unit);
  update_widget(g,mod_two.start);
  update_widget(g,mod_two.stop);
  update_widget(g,mod_two.current);
  update_widget(g,mod_two.current_lab);
  update_widget(g,mod_two.current_out);
  update_widget(g,mod_two.current_unit);
}

void add_pics(GUI* g,LOGGER* log)
{
  logger_log(log,"[GUI] Adding pictures");
  WIDGET* nmt=NULL;
  WIDGET* unm=NULL;

  nmt=create_picture(g,"res/robotics_lab.xpm",10,10);
  unm=create_picture(g,"res/unm_orth.xpm",415,10);

  add_to_main(g,nmt);
  add_to_main(g,unm);
  logger_log(log,"[GUI] Pictures Added");
}

void module(GUI* g, LOGGER* log,int num)
{
  char buf[1024];
  sprintf(buf,"[GUI] Creating Module %d Layout",num+1);
  logger_log(log,buf);
  WIDGET* border=NULL;
  WIDGET* sep1=NULL;
  WIDGET* sep2=NULL;
  WIDGET* enable=NULL;
  WIDGET* mode=NULL;
  WIDGET* force=NULL;
  WIDGET* desir=NULL;
  WIDGET* disp=NULL;
  WIDGET* unit=NULL;
  WIDGET* label=NULL;
  WIDGET* enter_field=NULL;
  WIDGET* start=NULL;
  WIDGET* stop=NULL;
  WIDGET* sep3=NULL;
  WIDGET* cur=NULL;
  WIDGET* out_label=NULL;
  WIDGET* output=NULL;
  WIDGET* out_unit=NULL;

  sprintf(buf,"Module %d",num+1);
  border=create_titled_border(buf,20+(330*num),130,280,300);
  set_titled_border_thickness(border,2);
  enable=create_checkbox("Enable Module",30+(330*num),140);
  sep1=create_seperator(30+(330*num),170,280);
  mode=create_label("Operation Mode:",30+(330*num),175);
  set_label_enable(mode,0);
  force=create_radio_button("Force Mode",40+(330*num),200);
  set_radio_button_check(force,1);
  set_radio_button_enable(force,0);
  disp=create_radio_button("Displacement Mode",40+(330*num),225);
  set_radio_button_enable(disp,0);
  sep2=create_seperator(30+(330*num),255,280);
  desir=create_label("Desired",30+(330*num),260);
  set_label_enable(desir,0);
  label=create_label("Force:",30+(330*num),280);
  set_label_enable(label,0);
  enter_field=create_textfield(155+(330*num),280,8);
  set_textfield_enable(enter_field,0);
  unit=create_label("lbs",235+(330*num),280);
  set_label_enable(unit,0);
  start=create_button("Start Module",30+(330*num),315);
  set_button_enable(start,0);
  stop=create_button("Stop Module",190+(330*num),315);
  set_button_enable(stop,0);
  sep3=create_seperator(30+(330*num),350,280);
  cur=create_label("Current",30+(330*num),355);  
  set_label_enable(cur,0);
  out_label=create_label("Force:",30+(330*num),375);
  set_label_enable(out_label,0);
  output=create_textfield(155+(330*num),375,8);
  set_textfield_enable(output,0);
  set_textfield_editable(output,0);
  out_unit=create_label("lbs",235+(330*num),375);
  set_label_enable(out_unit,0);

  add_to_main(g,border);
  add_to_main(g,enable);
  add_to_main(g,sep1);
  add_to_main(g,mode);
  add_to_main(g,force);
  add_to_main(g,disp);
  add_to_main(g,sep2);
  add_to_main(g,label);
  add_to_main(g,enter_field);
  add_to_main(g,desir);
  add_to_main(g,unit);
  add_to_main(g,start);
  add_to_main(g,stop);
  add_to_main(g,sep3);
  add_to_main(g,cur);
  add_to_main(g,out_label);
  add_to_main(g,output);
  add_to_main(g,out_unit);

  sprintf(buf,"[GUI] Module %d Created",num+1);
  logger_log(log,buf);

  if(num==0){
    mod_one.op_label=mode;
    mod_one.op_force=force;
    mod_one.op_disp=disp;
    mod_one.desired=label;
    mod_one.desired_lab=desir;
    mod_one.desired_in=enter_field;
    mod_one.desired_unit=unit;
    mod_one.start=start;
    mod_one.stop=stop;
    mod_one.current=cur;
    mod_one.current_lab=out_label;
    mod_one.current_out=output;
    mod_one.current_unit=out_unit;
    set_checkbox_callback(enable,mod_one_en,NULL);
    set_radio_button_callback(force,button_group_one,disp);
    set_radio_button_callback(disp,button_group_one,force);
  }
  else{
    mod_two.op_label=mode;
    mod_two.op_force=force;
    mod_two.op_disp=disp;
    mod_two.desired=label;
    mod_two.desired_lab=desir;
    mod_two.desired_in=enter_field;
    mod_two.desired_unit=unit;
    mod_two.start=start;
    mod_two.stop=stop;
    mod_two.current=cur;
    mod_two.current_lab=out_label;
    mod_two.current_out=output;
    mod_two.current_unit=out_unit;
    set_checkbox_callback(enable,mod_two_en,NULL);
    set_radio_button_callback(force,button_group_two,disp);
    set_radio_button_callback(disp,button_group_two,force);
  }
}

void ex_force_en(GUI* g,WIDGET* w,void* data)
{
  int en=get_checkbox_checked(w);
  set_label_enable(ex_force.label,en);
  set_textfield_enable(ex_force.output,en);
  set_label_enable(ex_force.unit,en);

  update_widget(g,ex_force.label);
  update_widget(g,ex_force.output);
  update_widget(g,ex_force.unit);
}

void extern_force(GUI* g,LOGGER* log)
{
  logger_log(log,"[GUI] Creating External Force Measurment Device");
  WIDGET* border=NULL;
  WIDGET* sep1=NULL;
  WIDGET* enable=NULL;
  WIDGET* label=NULL;
  WIDGET* output=NULL;
  WIDGET* unit=NULL;

  border=create_titled_border("External Force Device",700,130,110,200);
  set_titled_border_thickness(border,2);
  enable=create_checkbox("Enable Device",720,140);
  sep1=create_seperator(710,170,180);
  label=create_label("Measured Force:",720,175);
  set_label_enable(label,0);
  output=create_textfield(740,200,9);
  set_textfield_editable(output,0);
  set_textfield_enable(output,0);
  unit=create_label("lbs",840,200);
  set_label_enable(unit,0);

  set_checkbox_callback(enable,ex_force_en,NULL);

  ex_force.label=label;
  ex_force.output=output;
  ex_force.unit=unit;

  add_to_main(g,border);
  add_to_main(g,enable);
  add_to_main(g,sep1);
  add_to_main(g,label);
  add_to_main(g,output);
  add_to_main(g,unit);
  logger_log(log,"[GUI] External Force Measurment Device Created");
}

void ex_grip_en(GUI* g, WIDGET* w,void* data)
{
  int en=get_checkbox_checked(w);
  set_label_enable(ex_grip.disp_lab,en);
  set_textfield_enable(ex_grip.disp_out,en);
  set_label_enable(ex_grip.disp_unit,en);
  set_label_enable(ex_grip.force_lab,en);
  set_textfield_enable(ex_grip.force_out,en);
  set_label_enable(ex_grip.force_unit,en);

  update_widget(g,ex_grip.disp_lab);
  update_widget(g,ex_grip.disp_out);
  update_widget(g,ex_grip.disp_unit);
  update_widget(g,ex_grip.force_lab);
  update_widget(g,ex_grip.force_out);
  update_widget(g,ex_grip.force_unit);
}

void extern_grip(GUI* g, LOGGER* log)
{
  logger_log(log,"[GUI] Creating External Grip Measurment Device");
  WIDGET* border=NULL;
  WIDGET* sep1=NULL;
  WIDGET* enable=NULL;
  WIDGET* label=NULL;
  WIDGET* output=NULL;
  WIDGET* unit=NULL;
  WIDGET* cur_force=NULL;
  WIDGET* force_out=NULL;
  WIDGET* for_unit=NULL;

  border=create_titled_border("External Grip Device",700,260,150,230);
  set_titled_border_thickness(border,2);
  enable=create_checkbox("Enable Device",720,270);
  sep1=create_seperator(710,300,210);
  label=create_label("Current Displacement:",720,305);
  set_label_enable(label,0);
  output=create_textfield(740,330,9);
  set_textfield_editable(output,0);
  set_textfield_enable(output,0);
  unit=create_label("inches",840,330);
  set_label_enable(unit,0);
  cur_force=create_label("Measured Force:",720,355);
  set_label_enable(cur_force,0);
  force_out=create_textfield(740,380,9);
  set_textfield_editable(force_out,0);
  set_textfield_enable(force_out,0);
  for_unit=create_label("lbs",840,380);
  set_label_enable(for_unit,0);

  ex_grip.disp_lab=label;
  ex_grip.disp_out=output;
  ex_grip.disp_unit=unit;
  ex_grip.force_lab=cur_force;
  ex_grip.force_out=force_out;
  ex_grip.force_unit=for_unit;

  set_checkbox_callback(enable,ex_grip_en,NULL);

  add_to_main(g,border);
  add_to_main(g,enable);
  add_to_main(g,sep1);
  add_to_main(g,label);
  add_to_main(g,output);
  add_to_main(g,unit);
  add_to_main(g,cur_force);
  add_to_main(g,force_out);
  add_to_main(g,for_unit);
  logger_log(log,"[GUI] External Grip Measurment Device Created");
}

void start_call(GUI* g,WIDGET* w,void* data)
{
  set_label_text_color(data,0x0000FF00);
  set_label_text(data,"Running");
  update_widget(g,data);
}

void stop_call(GUI* g,WIDGET* w,void* data)
{
  char buf[1024];
  char str[1024];
  set_label_text_color(data,0x00FF0000);
  set_label_text(data,"Stopped");
  update_widget(g,data);
  getcwd(buf, 1024);
  sprintf(str,"%s/output.csv",buf);
  set_textfield_text(file,str);
  update_widget(g,file);
}

void start_all(GUI* g, LOGGER* log)
{
  logger_log(log,"[GUI] Creating File Output and Start-Stop Buttons");
  WIDGET* start=NULL;
  WIDGET* stop=NULL;
  WIDGET* label=NULL;
  WIDGET* stat_label=NULL;
  WIDGET* status=NULL;
  start=create_button("Start All Modules and Record Data",20,420);
  stat_label=create_label("Status:",420,420);
  status=create_label("Not Running",490,420);
  label=create_label("Output File:",20,455);
  file=create_textfield(140,455,93);
  set_textfield_editable(file,0);
  stop=create_button("Stop Modules and Write Data",720,420);

  set_button_callback(start,start_call,status);
  set_button_callback(stop,stop_call,status);

  add_to_main(g,start);
  add_to_main(g,label);
  add_to_main(g,stop);
  add_to_main(g,file);
  add_to_main(g,stat_label);
  add_to_main(g,status);
  logger_log(log,"[GUI] File Output and Start-Stop Buttons created");
}

void build_ui(GUI* g,LOGGER* log)
{
  create_main_window(g,"Hand Study Device");
  set_main_icon(g,"res/icons/main.xpm");
  set_main_size(g,500,1010);
  logger_log(log,"[GUI] Window built");
  add_pics(g,log);
  module(g,log,0);
  module(g,log,1);
  extern_force(g,log);
  extern_grip(g,log);
  start_all(g,log);
}
