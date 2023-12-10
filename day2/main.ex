defmodule Solution do

  def parse_cubes(c) do
  c |> Enum.map( &String.slice(&1, 1..-1) )
    |> Enum.map( &Integer.parse(&1) )
  end

  def parse_id("Game " <> id) do
    id |> Integer.parse()
       |> elem(0)
  end

  def game_possible(game) do
  game 
    |> Enum.at(1) 
    |> Enum.all?(
        &case &1 do
          {n, " red"  } -> n <= 12
          {n, " green"} -> n <= 13
          {n, " blue" } -> n <= 14
        end)
  end

  def part1(content) do
    lines = String.split(content, "\n")
    {_, lines} = List.pop_at(lines, -1)

    lines |> Enum.map( &String.split(&1, ":") )
          |> Enum.map( fn(s) -> 
              [ 
                s |> Enum.at(0) |> parse_id(),
                Regex.split( ~r/;|,/ , Enum.at(s, 1)) |> parse_cubes()
              ]
              end )
          |> Enum.filter( &game_possible(&1) )
          |> Enum.map( &Enum.at(&1, 0) )
          |> Enum.sum()
          |> IO.inspect()
  end
  def part2(content) do
  end
end

case File.read "puzzle.txt" do
  {:ok, content} ->
    # IO.puts(content)
    Solution.part1(content)
    Solution.part2(content)
  {:error} -> IO.puts("Failed to read puzzle.txt")
end
