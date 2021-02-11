require 'test_helper'

class EmailTest < ActiveSupport::TestCase
  # test "the truth" do
  #   assert true
  # end

  test "email must have user and value" do
	email = Email.new
	email.email_address = "whatever@whatever.what"
	email.user = User.find_by(username: "admin")
  end

  test "email must have existant user" do
	email = Email.new
	assert_not email.save
  end
end
