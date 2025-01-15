
use std::io::Write;
use std::io;
use std::str::SplitN;
use colored::Colorize;

// I don't know what this means at all...
// Except that the RL stands for Read Line. This is a macro to read line. 
macro_rules! RL { ($var:expr) => { let _ = io::stdin().read_line($var); }; }

// This is a Rust macro, obviously... But what's the () => {} stuff supposed to mean?
// And there's the relaxing difference between -> and => ...
macro_rules! FO { () => { io::stdout().flush().unwrap(); } }

// This macro takes a number and prints a > with certain indents.
// I'm probably getting hang of it.
macro_rules! IPI { ($num:expr) => { print!("{}> ", " ".repeat($num)); } }



/* ======= HR_SIM ======= */



struct Person {
	name: String,
	age: u64,
	place_of_origin: Place,
	education: String
}

struct Job {
	name: String,
	base_salary: f64,
	salary_incr_step: f64,
	level_cap: u64,
	base_value: f64,
	value_incr_step: f64
}

struct Staff {
	person: Person,
	job: Job,
	salary_offset: f64,
	value_offset: f64,
	greediness: f64
	willingness: f64,
	contribution: f64,
	potential: f64
}

struct Company {
	name: String,
	staff: Vec<Staff>,
	balance: f64,
	balance_history: Vec<f64> 
}

/* ====================== */



fn command_help(_args: &str) -> CommandReturn {
    let mut dialog_history: String = String::new();
    println!("{} {}", "[Help]", "Good, it seems that you have recovered the basic syntax of Rust!".bright_green());
    IPI!(2); FO!(); RL!(&mut dialog_history);

    println!("{} {}", "[Help]", "You'd probably want to stare at the code more to understand what 'unwrap_or()' is.".bright_green());
    IPI!(2); FO!(); RL!(&mut dialog_history);
    return "help".to_string();
}

fn command_unknown(_args: &str) -> CommandReturn {
    return "unknown".to_string();
}

fn command_exit(_args: &str) -> CommandReturn {
	return "exit".to_string();
}

type CommandReturn = String;
type CommandFunc = fn(&str) -> CommandReturn;

struct CommandInterface {
    func: CommandFunc,
    name: &'static str
}

impl CommandInterface {
	fn _new(func: CommandFunc, name: &'static str) -> Self {
		return Self { func, name };
	}
	
	fn _rename(&mut self, new_name: &'static str) -> Option<&'static str> {
		match new_name {
			"" => { return None; }
			_  => { self.name = new_name; return Some(new_name); }
		}
	}

	fn match_name(&self, input_name: &str) -> bool {
		let candidates: Vec<&str> = self.name.split('|').collect();
		for candidate in candidates {
			if input_name == candidate {
				return true;
			}
		}
		return false;
	} 
}

// Now that's how you create a list with type!
// ...: [type, count] = [...]
const COMMAND_LIST_MAIN: [CommandInterface; 3] = [
    CommandInterface {func: command_help,       name: "help|hallucination"},
    CommandInterface {func: command_exit, 		name: "quit|exit|leave|fuckoff"},
    CommandInterface {func: command_unknown,    name: "-_-"}
];

fn split_args(text: &str) -> (&str, &str) {
    let trimmed: &str = text.trim_start();
    let mut split: SplitN<'_, &str> = trimmed.splitn(2, " ");
    let first_part: &str = split.next().unwrap_or("");
    let second_part: &str = split.next().unwrap_or("");
    return (first_part, second_part);
}

/// Returns a tuple. The first value is whether the command is found, the second is what the command returns.
fn switch_command(text: &str, command_list: &[CommandInterface]) -> (bool, CommandReturn) {
    let last_line: &str = text.lines().last().unwrap_or("");
    let (command, args) = split_args(last_line);
    for ci in command_list.iter() {
        if ci.match_name(command) {
            return (true, (ci.func)(args));
        }
    }
    return (false, (command_list[command_list.len() - 1].func)(args));
}

fn speaking_shit() -> () {
    let mut dialog_history: String = String::new();
    println!("{}", "\n======= [ Press Enter to Continue ] ==========".bright_green());
    IPI!(2); FO!(); RL!(&mut dialog_history); // Also, an RL!() should be preceded by an immediate FO!().

    println!("{}", "What? You want to make a human resources sim?".bright_blue());
    IPI!(2); FO!(); RL!(&mut dialog_history); // You ask me why? That's how flushing works.

    println!("{}", "Well, damn... You suddenly forgot how to program in Rust.".bright_blue());
    IPI!(2); FO!(); RL!(&mut dialog_history);

    let _ = loop {
        println!("{}", "Don't worry. If you say 'help', everything are going to come back.".bright_blue());
        IPI!(2); FO!(); RL!(&mut dialog_history);
        let (success, retvl) = switch_command(&dialog_history, &COMMAND_LIST_MAIN);
        if success && (retvl == "help") { break retvl; }
    };

}

fn main_loop() -> () {
	let mut dialog_history: String = String::new();
	loop {
		println!();
		IPI!(2); FO!(); RL!(&mut dialog_history);
		if switch_command(&dialog_history, &COMMAND_LIST_MAIN).1 == "exit" {
			break;	
		}
	}
}

fn main() -> () {
    speaking_shit();
    main_loop();
}
